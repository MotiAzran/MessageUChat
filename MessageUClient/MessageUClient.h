#pragma once

#include "SocketStream.h"


class MessageUClient
{
public:
	MessageUClient();
	virtual ~MessageUClient() = default;

private:
	void _initialize_client_info();

	static SocketStream::Host _get_server_host();

private:
	SocketStream::Host _server_host;
	std::string _name;
	std::string _identifier;
	std::string _private_key;
};

