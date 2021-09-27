#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	class RegisterResponse : public Response
	{
	public:
		explicit RegisterResponse(Stream* stream);
		virtual ~RegisterResponse() = default;

		Types::ClientID client_id;
	};
}
