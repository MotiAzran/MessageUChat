#pragma once

#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	struct SendMessageResponse : public Response
	{
		explicit SendMessageResponse(Stream* stream);
		virtual ~SendMessageResponse() = default;

		Types::ClientID client_id;
		uint32_t message_id;
	};
}

