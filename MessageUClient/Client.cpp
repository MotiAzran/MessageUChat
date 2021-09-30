#include <fstream>
#include "StringUtils.h"
#include "Serializer.h"
#include "Base64Wrapper.h"
#include "ProtocolUtils.h"
#include "Exceptions.h"
#include "Client.h"

// Requests
#include "GetClientsListRequest.h"
#include "GetPublicKeyRequest.h"
#include "GetWaitingMessagesRequest.h"
#include "SendTextMessageRequest.h"
#include "RequestSymetricKeyRequest.h"
#include "SendSymetricKeyRequest.h"

// Responses
#include "ClientsListResponse.h"
#include "GetPublicKeyResponse.h"
#include "GetWaitingMessagesResponse.h"
#include "SendMessageResponse.h"

Client* Client::client_from_file(const std::filesystem::path& path)
{
	std::ifstream info_file(path);
	if (!info_file.is_open() || 0 == std::filesystem::file_size(path))
	{
		throw std::exception("Client info not found");
	}

	// Get name from file first line
	std::string name;
	std::getline(info_file, name);
	if (name.size() > Common::MAX_CLIENT_NAME_LENGTH)
	{
		throw std::exception("Invalid file name");
	}

	// Get client ID from file second line
	std::string identifier;
	std::getline(info_file, identifier);
	if (identifier.size() != Common::CLIENT_ID_STR_LENGTH)
	{
		throw std::exception("Invalid user indentifier");
	}

	// Get private key from the rest of the file
	std::string encoded_private_key(std::istreambuf_iterator<char>(info_file), {});
	auto private_key = Base64Wrapper::decode(encoded_private_key);

	return new Client(name, StringUtils::to_client_id(identifier), private_key);
}

Client::Client(const std::string& name, const Types::ClientID& identifier, const std::string& private_key) :
	_name(name),
	_id(identifier),
	_rsapriv(private_key),
	_aes() {}

void Client::get_clients_list(const Types::Host& host)
{
	SocketStream sock(host);

	Protocol::GetClientsListRequest request(_id, Common::VERSION);
	sock.write(request.serialize());

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

	Protocol::GetPublicKeyRequest request(_id, Common::VERSION, client.id);
	sock.write(request.serialize());

	// Receive response
	Protocol::GetPublicKeyResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}

	_clients.update(client.id, response.rsapub);
}

void Client::get_waiting_messages(const Types::Host& host)
{
	SocketStream sock(host);

	Protocol::GetWaitingMessagesRequest request(_id, Common::VERSION);
	sock.write(request.serialize());

	Protocol::GetWaitingMessagesResponse response(&sock);
	if (response.is_done())
	{
		std::cout << "No waiting messages" << std::endl;
	}

	while (!response.is_done())
	{
		_print_message(response.read_next_message());
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

	SocketStream sock(host);

	Protocol::SendTextMessageRequest request(_id, Common::VERSION, client.id, client.aes.encrypt(message));
	sock.write(request.serialize());

	Protocol::SendMessageResponse response(&sock);
	if (response.client_id != client.id)
	{
		throw ServerErrorException();
	}
}

void Client::request_symetric_key(const Types::Host& host)
{
	auto client = _get_client_from_user();

	SocketStream sock(host);

	Protocol::RequestSymetricKeyRequest request(_id, Common::VERSION, client.id);
	sock.write(request.serialize());

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

	SocketStream sock(host);

	Protocol::SendSymetricKeyRequest request(_id, Common::VERSION, client.id, client.rsapub.encrypt(_aes.getKey()));
	sock.write(request.serialize());

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

void Client::_print_message(Protocol::MessageEntry&& message)
{
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
