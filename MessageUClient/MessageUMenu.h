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

	void add_client_id(const std::string& name, const Types::ClientID& id);
	Types::ClientID get_client_id(const std::string& name) const;

	void add_public_key(const Types::ClientID& id, const std::string& public_key) { _id_to_pubkey[id] = public_key; }
	std::string get_public_key(const Types::ClientID& id) const;

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
	std::map<std::string, Types::ClientID> _name_to_id;
	std::map<Types::ClientID, std::string> _id_to_pubkey;
};
