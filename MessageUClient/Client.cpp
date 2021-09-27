#include <sstream>
#include "StringUtils.h"
#include "Serializer.h"
#include "Base64Wrapper.h"
#include "ProtocolUtils.h"
#include "Exceptions.h"
#include "ClientsListResponse.h"
#include "GetPublicKeyResponse.h"
#include "GetWaitingMessagesResponse.h"
#include "SendMessageResponse.h"
#include "Client.h"

std::string decode_private_key(const std::string& encoded_private_key)
{
	return Base64Wrapper::decode(encoded_private_key);
}

Client* Client::client_from_file(FileStream& info_file)
{
	auto info = info_file.read(info_file.get_file_size());
	if (info.empty())
	{
		throw std::exception("Client info not found");
	}
	
	std::stringstream info_stream(info);

	// Get name from file first line
	std::string name;
	std::getline(info_stream, name);
	if (name.size() > Common::MAX_CLIENT_NAME_LENGTH)
	{
		throw std::exception("Invalid file name");
	}

	// Get client ID from file second line
	std::string identifier;
	std::getline(info_stream, identifier);
	if (identifier.size() != Common::CLIENT_ID_STR_LENGTH)
	{
		throw std::exception("Invalid user indentifier");
	}

	// Get private key from the rest of the file
	std::string encoded_private_key(std::istreambuf_iterator<char>(info_stream), {});
	auto private_key = decode_private_key(encoded_private_key);

	return new Client(name, StringUtils::to_client_id(identifier), private_key);
}

Client* Client::client_from_file(FileStream&& info_file)
{
	return Client::client_from_file(info_file);
}

Client::Client(const std::string& name, const Types::ClientID& identifier, const std::string& private_key) :
	_name(name),
	_id(identifier),
	_rsapriv(private_key),
	_aes() {}

void Client::get_clients_list(const Types::Host& host)
{
	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::ClientsList, 0);
	Protocol::ClientsListResponse response(&sock);
	if (response.is_done())
	{
		std::cout << "No other clients" << std::endl;
		return;
	}

	while (!response.is_done())
	{
		auto client_entry = response.get_next_entry();

		std::cout << "- " << client_entry.name << std::endl;

		_clients.insert(client_entry.id, client_entry.name);
	}
}

void Client::get_client_public_key(const Types::Host& host)
{
	// Send request
	auto client = _get_client_from_user();

	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::PublicKeyRequest, Common::CLIENT_ID_SIZE_BYTES);
	sock.write(StringUtils::to_string(client.id));

	// Receive response
	Protocol::GetPublicKeyResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}

	_clients.update(client.id, response.public_key);
}

void Client::get_waiting_messages(const Types::Host& host)
{
	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::WaitingMessagesRequest, 0);

	Protocol::GetWaitingMessagesResponse response(&sock);
	if (response.is_done())
	{
		std::cout << "No waiting messages" << std::endl;
	}

	while (!response.is_done())
	{
		auto message = response.read_next_message();
		auto client = _clients.get_client(message.client_id);
		std::cout << "From: " << client.name << std::endl;
		std::cout << "Content:\n";
		try
		{
			switch (message.message_type)
			{
			case Protocol::MessageType::SendSymetricKey:
				_clients.update(message.client_id, StringUtils::to_aes_key(_rsapriv.decrypt(message.message_content)));
				std::cout << "Symetric key received";
				break;
			case Protocol::MessageType::SymetricKeyRequest:
				std::cout << "Request for symetric key";
				break;
			case Protocol::MessageType::Text:
				std::cout << _aes.decrypt(message.message_content);
				break;
			default:
				std::cout << "Can't decrypt message";
			}
		} 
		catch (...)
		{
			std::cout << "Can't decrypt message";
		}
		std::cout << "\n-----<EOM>-----\n" << std::endl;
	}
}

void Client::send_text_message(const Types::Host& host)
{
	auto client = _get_client_from_user();
	if (!_clients.has_val<Types::AESKey>(client.name))
	{
		std::cout << "Request for AES key and try again" << std::endl;
		return;
	}

	std::string message;
	std::cout << "Enter message: ";
	std::getline(std::cin, message);

	Serializer payload;
	auto encrypted_message = AESWrapper(client.aes_key).encrypt(message);
	payload.add(client.id, Protocol::MessageType::Text, static_cast<uint32_t>(encrypted_message.size()), encrypted_message);

	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::SendMessageToUser, payload.get_serialized_data_size());
	sock.write(payload.serialize());

	Protocol::SendMessageResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}
}

void Client::request_symetric_key(const Types::Host& host)
{
	auto client = _get_client_from_user();
	Serializer payload;
	payload.add(client.id, Protocol::MessageType::SymetricKeyRequest, static_cast<uint32_t>(0));

	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::SendMessageToUser, payload.get_serialized_data_size());
	sock.write(payload.serialize());

	Protocol::SendMessageResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}
}

void Client::send_symetric_key(const Types::Host& host)
{
	auto client = _get_client_from_user();
	if (!_clients.has_val<Types::PublicKey>(client.id))
	{
		std::cout << "Get client public key and try again" << std::endl;
		return;
	}

	Serializer payload;
	auto encrypted_key = RSAPublicWrapper(client.public_key).encrypt(_aes.getKey());
	payload.add(client.id, Protocol::MessageType::SendSymetricKey, static_cast<uint32_t>(encrypted_key.size()), encrypted_key);

	SocketStream sock(host);
	Protocol::Utils::send_request_header(&sock, _id, Protocol::RequestCode::SendMessageToUser, payload.get_serialized_data_size());
	sock.write(payload.serialize());

	Protocol::SendMessageResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}
}

ClientsList::ClientField Client::_get_client_from_user()
{
	char client_name[Common::MAX_CLIENT_NAME_LENGTH] = { 0 };
	std::cout << "Enter client name: ";
	std::cin.ignore();
	std::cin.getline(client_name, Common::MAX_CLIENT_NAME_LENGTH);

	return _clients.get_client(std::string(client_name));
}
