#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	struct RegisterResponse : public Response
	{
		explicit RegisterResponse(Stream* stream);
		virtual ~RegisterResponse() = default;

		Types::ClientID client_id;
	};
}
