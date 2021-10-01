#pragma once

#include <type_traits>
#include "Request.h"
#include "Response.h"
#include "Socket.h"

namespace Protocol
{
	template <typename T, typename = std::enable_if_t<std::is_base_of_v<Request, T>>>
	void send_request(Socket& sock, T&& request)
	{
		sock.send(request.serialize());
	}

	template <typename T, typename = std::enable_if_t<std::is_base_of_v<Response, T>>>
	T receive_response(Socket& sock)
	{
		return T(std::bind(&Socket::receive, &sock, std::placeholders::_1));
	}
}
