#pragma once

#include "Stream.h"
#include "IPAddress.h"
#include "Common.h"

class SocketStream : public Stream
{
public:
	using Port = uint16_t;
	using Host = std::pair<IPAddress, Port>;

public:
	explicit SocketStream(SOCKET sock);
	explicit SocketStream(const Host& host);
	virtual ~SocketStream();
	SocketStream(const SocketStream&) = delete;
	SocketStream& operator=(const SocketStream&) = delete;
	SocketStream(SocketStream&& other) noexcept;
	SocketStream& operator=(SocketStream&& other) noexcept;

	virtual Buffer read(const uint32_t size) override;
	virtual void write(const Buffer& buf) override;

private:
	static SOCKET _connect_to_host(const Host& host);

private:
	SOCKET _sock;
};
