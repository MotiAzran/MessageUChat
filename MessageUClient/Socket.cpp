#include "Socket.h"

Socket::Socket(SOCKET sock) :
	_sock(sock)
{
	if (INVALID_SOCKET == sock)
	{
		throw std::exception("Got invalid socket value");
	}
}

Socket::Socket(const Host& host) :
	Socket(_connect_to_host(host)) {}

Socket::~Socket()
{
	try
	{
		if (INVALID_SOCKET != _sock)
		{
			closesocket(_sock);
			shutdown(_sock, SD_BOTH);
		}
	} catch(...) {}
}

Socket::Socket(Socket&& other) noexcept :
	_sock(other._sock)
{
	other._sock = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& other) noexcept
{
	_sock = other._sock;
	other._sock = INVALID_SOCKET;

	return *this;
}

std::string Socket::receive(const uint32_t size)
{
	if (0 == size)
	{
		return std::string();
	}

	std::string buf(size, 0);

	if (SOCKET_ERROR == ::recv(_sock, reinterpret_cast<char*>(buf.data()), size, MSG_WAITALL))
	{
		throw std::exception("Receive data failed");
	}

	return buf;
}

void Socket::send(const std::string& data)
{
	if (data.empty())
	{
		throw std::exception("Can't send empty buffer");
	}

	if (SOCKET_ERROR == ::send(_sock, data.data(), data.size(), 0))
	{
		throw std::exception("Send data failed");
	}
}

SOCKET Socket::_connect_to_host(const Host& host)
{
	SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock)
	{
		throw std::exception("socket initilaization error");
	}

	struct sockaddr_in sock_addr = { 0 };
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(std::get<Types::Port>(host));
	sock_addr.sin_addr.s_addr = std::get<IPAddress>(host).get_ip();

	if (SOCKET_ERROR == ::connect(sock, reinterpret_cast<const sockaddr*>(&sock_addr), sizeof(sock_addr)))
	{
		throw std::exception("Server unreachable");
	}

	return sock;
}
