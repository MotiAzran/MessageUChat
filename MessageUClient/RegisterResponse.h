#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	struct RegisterResponse
	{
		explicit RegisterResponse(Response&& response);
		virtual ~RegisterResponse() = default;

		Types::ClientID client_id;
	};
}
