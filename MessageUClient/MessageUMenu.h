#pragma once

#include <unordered_map>
#include "ActionFactory.h"
#include "SocketStream.h"
#include "Client.h"
#include "Types.h"

// Actions
#include "RegisterAction.h"
#include "ClientsListAction.h"
#include "ExitAction.h"

class MessageUMenu
{
public:
	MessageUMenu();
	virtual ~MessageUMenu();

	void print_menu();
	void handle_choice(const int choice);

	Types::Host get_server_host() { return _server_host; }

	Client* get_client() { return _client; }
	void set_client(Client* client) { _client = client; }

private:
	static Types::Host _get_server_host_from_file();
	static Client* _get_client_from_file();

	std::unordered_map<int, IActionFactory*> _actions_factory = {
		{10, new ActionFactory<RegisterAction>},
		{20, new ActionFactory<ClientsListAction>},
		{0, new ActionFactory<ExitAction>}
	};

private:
	Types::Host _server_host;
	Client* _client;
};
