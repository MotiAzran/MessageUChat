#include <iostream>
#include <filesystem>
#include "MessageUMenu.h"
#include "SocketStream.h"
#include "FileStream.h"
#include "Serializer.h"
#include "RSAWrapper.h"
#include "HexWrapper.h"
#include "Base64Wrapper.h"
#include "ProtocolUtils.h"
#include "RegisterAction.h"

const std::string RegisterAction::MENU_NAME = "Register";

void RegisterAction::execute(MessageUMenu& menu)
{
	if (!_can_register())
	{
		throw std::exception("You already registered");
	}

	char username[Common::MAX_USER_NAME_LENGTH];
	std::cout << "Enter user name: ";
	std::cin.ignore();
	std::cin.getline(username, Common::MAX_USER_NAME_LENGTH);

	// Create new client rsa key pair
	RSAPrivateWrapper rsapriv;

	// Create connection with the client
	SocketStream sock(menu.get_server_host());
	_send_request(sock, username, rsapriv);
	
	auto client_id = _get_response(sock);

	_write_client_info(username, client_id, rsapriv);

	menu.set_client(Client::client_from_file(FileStream(Common::CLIENT_INFO_FILE_PATH)));
}

bool RegisterAction::_can_register()
{
	if (std::filesystem::exists(Common::CLIENT_INFO_FILE_PATH))
	{
		return false;
	}

	return true;
}

void RegisterAction::_send_request(SocketStream& sock, const std::string& username, const RSAPrivateWrapper& rsapriv)
{
	Serializer payload;

	// Add username and public key to payload
	std::string padded_username(username);
	padded_username.resize(Common::MAX_USER_NAME_LENGTH);
	payload.add(padded_username, rsapriv.getPublicKey());

	// Send request
	Protocol::Utils::send_request_header(sock, Types::ClientID(), Protocol::RequestCode::Register, payload.get_serialized_data_size());
	sock.write(payload.serialize());
}

Types::ClientID RegisterAction::_get_response(SocketStream& sock)
{
	// Read response header
	auto header = Protocol::Utils::get_response_header(sock);
	if (Protocol::ResponseCode::RegisterSuccess != header.code)
	{
		throw std::exception("Registration failed");
	}

	auto payload = sock.read(header.payload_size);

	return StringUtils::to_client_id(payload);
}

void RegisterAction::_write_client_info(const std::string& username, const Types::ClientID& id, const RSAPrivateWrapper& rsapriv)
{
	FileStream info_file(Common::CLIENT_INFO_FILE_PATH);

	info_file.write(username + "\n");

	std::string encoded_id = HexWrapper::encode(StringUtils::to_string(id));
	info_file.write(encoded_id + "\n");

	std::string encoded_private_key = Base64Wrapper::encode(rsapriv.getPrivateKey());
	info_file.write(encoded_private_key);
}
