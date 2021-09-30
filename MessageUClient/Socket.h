#pragma once

#include "IPAddress.h"
#include "StringUtils.h"
#include "Types.h"

using Types::Host;

class Socket
{
public:
	explicit Socket(SOCKET sock);
	explicit Socket(const Host& host);
	virtual ~Socket();
	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;
	Socket(Socket&& other) noexcept;
	Socket& operator=(Socket&& other) noexcept;

	std::string receive(const uint32_t size);
	void send(const std::string& data);

private:
	static SOCKET _connect_to_host(const Host& host);

private:
	SOCKET _sock;
};
