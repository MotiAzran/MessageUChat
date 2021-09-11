#pragma once

#include "SocketStream.h"


class MessageUClient
{
public:
	MessageUClient();
	virtual ~MessageUClient() = default;

private:
	static SocketStream::Host _get_server_host();

private:
	SocketStream::Host _server_host;
};

