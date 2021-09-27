#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileStream.h"
#include "SocketStream.h"
#include "StringUtils.h"
#include "Exceptions.h"
#include "ProtocolUtils.h"
#include "RegisterResponse.h"
#include "HexWrapper.h"
#include "Base64Wrapper.h"
#include "MessageUMenu.h"

MessageUMenu::MessageUMenu() :
	_server_host(_get_server_host_from_file()),
	_client(_get_client_from_file())
{
	_choice_handlers = {
		{MenuChoice::Register, {std::bind(&MessageUMenu::_register, this), "Register"}},
		{MenuChoice::ClientsListRequest, {_get_client_action(&Client::get_clients_list), "Request for clients list"}},
		{MenuChoice::PublicKeyRequest, {_get_client_action(&Client::get_client_public_key), "Request for public key"}},
		{MenuChoice::WaitingMessagesRequest, {_get_client_action(&Client::get_waiting_messages), "Request for waiting messages"}},
		{MenuChoice::SendTextMessage, {_get_client_action(&Client::send_text_message), "Send a text message"}},
		{MenuChoice::SymetricKeyRequest, {_get_client_action(&Client::request_symetric_key), "Send a request for a symetric key"}},
		{MenuChoice::SendSymetricKey, {_get_client_action(&Client::send_symetric_key), "Send your symetric key"}},
		{MenuChoice::Exit, {[](const auto...) { throw ExitException(); }, "Exit client"}}
	};
}

MessageUMenu::~MessageUMenu()
{
	try
	{
		if (nullptr != _client)
		{
			delete _client;
		}
	} catch(...) {}
}

void MessageUMenu::print_menu()
{
	for (auto& [menu_index, val] : _choice_handlers)
	{
		auto& [func, menu_desc] = val;
		std::cout << std::setw(2) << static_cast<uint32_t>(menu_index) << ") " << menu_desc << std::endl;
	}
}

void MessageUMenu::handle_choice(const uint32_t choice)
{
	std::cout << std::endl;
	_get_choice_handler(static_cast<MenuChoice>(choice))();
	std::cout << std::endl;
}

void MessageUMenu::_register()
{
	if (nullptr != _client)
	{
		std::cout << "You already registered" << std::endl;
		return;
	}

	char client_name[Common::MAX_CLIENT_NAME_LENGTH] = { 0 };
	std::cout << "Enter client name: ";
	std::cin.ignore();
	std::cin.getline(client_name, Common::MAX_CLIENT_NAME_LENGTH);

	// Create new client rsa key pair
	RSAPrivateWrapper rsapriv;

	// Create connection with the server
	SocketStream sock(_server_host);

	// Send request
	Protocol::Utils::send_request_header(&sock, Types::ClientID(), Protocol::RequestCode::Register, Common::MAX_CLIENT_NAME_LENGTH + Common::PUBLIC_KEY_SIZE);
	sock.write(client_name, Common::MAX_CLIENT_NAME_LENGTH);
	sock.write(rsapriv.getPublicKey());

	Protocol::RegisterResponse response(&sock);

	_client = new Client(client_name, response.client_id, rsapriv.getPrivateKey());
	_write_client_info();
}

void MessageUMenu::_write_client_info()
{
	if (nullptr == _client)
	{
		throw std::exception("Unexpected error!");
	}

	FileStream info_file(Common::CLIENT_INFO_FILE_PATH);

	info_file.write(_client->get_name() + "\n");

	std::string encoded_id = HexWrapper::encode(StringUtils::to_string(_client->get_id()));
	info_file.write(encoded_id + "\n");

	std::string encoded_private_key = Base64Wrapper::encode(_client->get_private_key());
	info_file.write(encoded_private_key);
}

MessageUMenu::ActionFunc MessageUMenu::_get_client_action(const Client::ActionFunc& func)
{
	return [this, func]() {
		if (nullptr == this->get_client())
		{
			std::cout << "Register and try again" << std::endl;
			return;
		}

		try
		{
			std::invoke(func, this->get_client(), this->get_server_host());
		} 
		catch (const ExitException&)
		{
			throw;
		}
		catch (const std::exception & ex)
		{
			std::cout << ex.what() << std::endl;
		}
	};
}

MessageUMenu::ActionFunc MessageUMenu::_get_choice_handler(const MenuChoice choice)
{
	auto it = std::find_if(_choice_handlers.begin(), _choice_handlers.end(),
		[choice](const auto val) {return std::get<MenuChoice>(val) == choice; });
	if (_choice_handlers.end() == it)
	{
		return []() { std::cout << "Invalid choice" << std::endl; };
	}

	return std::get<ActionFunc>(it->second);
}

Types::Host MessageUMenu::_get_server_host_from_file()
{
	if (!std::filesystem::exists(Common::SERVER_INFO_PATH))
	{
		throw std::exception("Server info not found");
	}

	FileStream server_info(Common::SERVER_INFO_PATH);
	auto info = server_info.read(server_info.get_file_size());
	if (info.empty())
	{
		throw std::exception("Server info not found");
	}

	std::stringstream info_stream(info);

	std::string ip_str;
	std::getline(info_stream, ip_str, ':');

	std::string port_str;
	info_stream >> port_str;
	if (!StringUtils::is_number(port_str))
	{
		throw std::exception("Invalid port");
	}

	auto port = std::stoi(port_str);
	if (0 > port || Common::MAX_PORT < port)
	{
		throw std::exception("Invalid port value");
	}

	return Types::Host(IPAddress(ip_str), static_cast<Types::Port>(port));
}

Client* MessageUMenu::_get_client_from_file()
{
	if (std::filesystem::exists(Common::CLIENT_INFO_FILE_PATH))
	{
		return Client::client_from_file(FileStream(Common::CLIENT_INFO_FILE_PATH));
	}

	return nullptr;
}
