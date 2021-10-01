#pragma once

#include <vector>
#include <utility>
#include "Client.h"
#include "Types.h"

class MessageUMenu
{
	using HandlerFunc = std::function<void()>;
	using ChoicePair = std::pair<HandlerFunc, std::string>;

	enum class MenuChoice : uint32_t
	{
		Exit = 0,
		Register = 10,
		ClientsListRequest = 20,
		PublicKeyRequest = 30,
		WaitingMessagesRequest = 40,
		SendTextMessage = 50,
		SymetricKeyRequest,
		SendSymetricKey
	};

public:
	MessageUMenu();
	virtual ~MessageUMenu();

	Client* get_client() const { return _client; }
	Types::Host get_server_host() const { return _server_host; }

	void print() const;
	void handle_choice(const uint32_t choice);

private:
	void _register();
	bool _is_client_name_valid(const std::string& name);
	std::string _get_client_name();
	void _write_client_info();
	HandlerFunc _generate_client_handler_func(const Client::HandlerFunc& func);
	HandlerFunc _get_choice_handler(const MenuChoice choice);

	static Types::Host _get_server_host_from_file();
	static Client* _get_client_from_file();

private:
	std::vector<std::pair<MenuChoice, ChoicePair>> _choice_handlers;

	Types::Host _server_host;
	Client* _client;
};
