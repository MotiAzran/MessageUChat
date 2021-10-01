#pragma once

#include <cstdint>
#include <string>
#include "ProtocolCommon.h"
#include "RSAWrapper.h"
#include "AESWrapper.h"
#include "ClientsList.h"
#include "Types.h"

class Client
{
public:
	using HandlerFunc = void(Client::*)(const Types::Host&);

	static Client* client_from_file(const std::filesystem::path& path);

public:
	Client(const std::string& name, const Types::ClientID& identifier, const std::string& private_key);
	virtual ~Client() = default;
	Client(const Client&) = delete;
	Client& operator=(const Client&) = delete;

	std::string get_name() const { return _name; }
	Types::ClientID get_id() const { return _id; }
	std::string get_private_key() const { return _rsapriv.getPrivateKey(); }
	Types::PublicKey get_public_key() const { return _rsapriv.getPublicKey(); }

	// Handlers
	void get_clients_list(const Types::Host& host);
	void get_client_public_key(const Types::Host& host);
	void get_waiting_messages(const Types::Host& host);
	void send_text_message(const Types::Host& host);
	void request_symetric_key(const Types::Host& host);
	void send_symetric_key(const Types::Host& host);

private:
	ClientsList::ClientField _get_client_from_user();
	void _handle_message(Protocol::MessageEntry&& message);

private:
	std::string _name;
	Types::ClientID _id;
	RSAPrivateWrapper _rsapriv;
	AESWrapper _aes;
	ClientsList _clients;
};
