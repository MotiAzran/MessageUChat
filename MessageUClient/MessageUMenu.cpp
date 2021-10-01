#include <iostream>
#include <fstream>
#include <filesystem>
#include "Socket.h"
#include "StringUtils.h"
#include "Exceptions.h"
#include "RegisterRequest.h"
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
		{MenuChoice::ClientsListRequest, {_generate_client_handler_func(&Client::get_clients_list), "Request for clients list"}},
		{MenuChoice::PublicKeyRequest, {_generate_client_handler_func(&Client::get_client_public_key), "Request for public key"}},
		{MenuChoice::WaitingMessagesRequest, {_generate_client_handler_func(&Client::get_waiting_messages), "Request for waiting messages"}},
		{MenuChoice::SendTextMessage, {_generate_client_handler_func(&Client::send_text_message), "Send a text message"}},
		{MenuChoice::SymetricKeyRequest, {_generate_client_handler_func(&Client::request_symetric_key), "Send a request for a symetric key"}},
		{MenuChoice::SendSymetricKey, {_generate_client_handler_func(&Client::send_symetric_key), "Send your symetric key"}},
		{MenuChoice::Exit, {[](const auto&...) { throw ExitException(); }, "Exit client"}}
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

void MessageUMenu::print() const
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

	auto client_name = _get_client_name();

	// Create new client RSA key pair
	RSAPrivateWrapper rsapriv;

	// Create connection with the server
	Socket sock(_server_host);

	// Send request
	Protocol::RegisterRequest request(Types::ClientID(), Common::VERSION, client_name, rsapriv.getPublicKey());
	sock.send(request.serialize());

	Protocol::RegisterResponse response(std::bind(&Socket::receive, &sock, std::placeholders::_1));

	_client = new Client(client_name, response.client_id, rsapriv.getPrivateKey());
	_write_client_info();
}

bool MessageUMenu::_is_client_name_valid(const std::string& name)
{
	const char MIN_VALID_CHARACTER = 32, MAX_VALID_CAHARACTER = 127;

	if (name.empty())
	{
		// Empty client name
		return false;
	}

	if (!std::any_of(name.begin(), name.end(), std::isalpha))
	{
		// Client name without letters
		return false;
	}

	if (!std::all_of(name.begin(), name.end(),
		[MIN_VALID_CHARACTER, MAX_VALID_CAHARACTER](const char c) { return MIN_VALID_CHARACTER <= c && MAX_VALID_CAHARACTER >= c; }))
	{
		// Client name with character out of the range
		return false;
	}

	return true;
}

std::string MessageUMenu::_get_client_name()
{
	char client_name[Common::MAX_CLIENT_NAME_LENGTH] = { 0 };

	std::cout << "Enter client name: ";
	std::cin.getline(client_name, Common::MAX_CLIENT_NAME_LENGTH);

	while (!_is_client_name_valid(client_name))
	{
		std::cout << "Invalid client name!" << std::endl;
		std::cout << "Enter client name: ";
		std::cin.getline(client_name, Common::MAX_CLIENT_NAME_LENGTH);
	}
	
	return client_name;
}

void MessageUMenu::_write_client_info()
{
	if (nullptr == _client)
	{
		throw std::exception("Unexpected error!");
	}

	std::ofstream info_file(Common::CLIENT_INFO_FILE_PATH);

	// Write client name
	info_file << _client->get_name() << std::endl;

	// Write client ID
	std::string encoded_id = HexWrapper::encode(StringUtils::to_string(_client->get_id()));
	info_file << encoded_id << std::endl;

	// Write client private key
	std::string encoded_private_key = Base64Wrapper::encode(_client->get_private_key());
	info_file << encoded_private_key;
}

MessageUMenu::HandlerFunc MessageUMenu::_generate_client_handler_func(const Client::HandlerFunc& func)
{
	return [this, func]() {
		// Check client registered
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
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	};
}

MessageUMenu::HandlerFunc MessageUMenu::_get_choice_handler(const MenuChoice choice)
{
	// Search for user choice handler
	auto it = std::find_if(_choice_handlers.begin(), _choice_handlers.end(),
		[choice](const auto val) {return std::get<MenuChoice>(val) == choice; });
	if (_choice_handlers.end() == it)
	{
		return [](const auto&...) { std::cout << "Invalid choice" << std::endl; };
	}

	return std::get<HandlerFunc>(it->second);
}

Types::Host MessageUMenu::_get_server_host_from_file()
{
	const auto MAX_PORT = 0xffff;

	if (!std::filesystem::exists(Common::SERVER_INFO_PATH))
	{
		throw std::exception("Server info not found");
	}

	std::ifstream server_info(Common::SERVER_INFO_PATH);
	if (!server_info.is_open() || 0 == std::filesystem::file_size(Common::SERVER_INFO_PATH))
	{
		throw std::exception("Server info not found");
	}

	// Read IP address from file
	std::string ip_str;
	std::getline(server_info, ip_str, ':');

	// Read port from file
	std::string port_str;
	server_info >> port_str;
	if (!StringUtils::is_number(port_str))
	{
		throw std::exception("Invalid port");
	}

	auto port = std::stoi(port_str);
	if (0 > port || MAX_PORT < port)
	{
		throw std::exception("Invalid port value");
	}

	return Types::Host(IPAddress(ip_str), static_cast<Types::Port>(port));
}

Client* MessageUMenu::_get_client_from_file()
{
	if (std::filesystem::exists(Common::CLIENT_INFO_FILE_PATH))
	{
		return Client::client_from_file(Common::CLIENT_INFO_FILE_PATH);
	}

	return nullptr;
}
