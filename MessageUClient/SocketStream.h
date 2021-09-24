#pragma once

#include "Stream.h"
#include "IPAddress.h"
#include "StringUtils.h"
#include "Types.h"

using Types::Host;

class SocketStream : public Stream
{
public:
	explicit SocketStream(SOCKET sock);
	explicit SocketStream(const Host& host);
	virtual ~SocketStream();
	SocketStream(const SocketStream&) = delete;
	SocketStream& operator=(const SocketStream&) = delete;
	SocketStream(SocketStream&& other) noexcept;
	SocketStream& operator=(SocketStream&& other) noexcept;

	virtual std::string read(const uint32_t size) override;
	virtual void write(const std::string& data) override;

	template <typename T, typename = std::enable_if_t<std::is_pod_v<T>>>
	T read_pod()
	{
		return StringUtils::to_pod<T>(read(sizeof(T)));
	}

private:
	static SOCKET _connect_to_host(const Host& host);

private:
	SOCKET _sock;
};
