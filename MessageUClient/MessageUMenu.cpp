#include <iostream>
#include <sstream>
#include <filesystem>
#include "FileStream.h"
#include "StringUtils.h"
#include "MessageUMenu.h"

MessageUMenu::MessageUMenu() :
	_server_host(_get_server_host_from_file()),
	_client(_get_client_from_file()) {}

MessageUMenu::~MessageUMenu()
{
	try
	{
		if (nullptr != _client)
		{
			delete _client;
		}

		for (auto [index, factory] : _actions_factory)
		{
			delete factory;
		}
	} catch(...) {}
}

void MessageUMenu::print_menu()
{
	for (auto& [menu_index, factory] : _actions_factory)
	{
		std::cout << std::setw(2) << menu_index << ") " << factory->get_menu_name() << std::endl;
	}
}

void MessageUMenu::handle_choice(const int choice)
{
	if (_actions_factory.find(choice) == _actions_factory.cend())
	{
		std::cout << "Invalid choice" << std::endl << std::endl;
		return;
	}

	auto handler = _actions_factory.at(choice)->create();

	std::cout << std::endl;
	handler->execute(*this);
	std::cout << std::endl;
}

void MessageUMenu::add_client_id(const std::string& name, const Types::ClientID& id)
{
	_name_to_id[name] = id;
}

Types::ClientID MessageUMenu::get_client_id(const std::string& name) const
{
	if (_name_to_id.find(name) == _name_to_id.cend())
	{
		throw std::exception("Client not exists");
	}

	return _name_to_id.at(name);
}

std::string MessageUMenu::get_public_key(const Types::ClientID& id) const
{
	if (_id_to_pubkey.find(id) == _id_to_pubkey.cend())
	{
		throw std::exception("Client not exists");
	}

	return _id_to_pubkey.at(id);
}

Types::Host MessageUMenu::_get_server_host_from_file()
{
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
