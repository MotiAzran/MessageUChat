#pragma once

#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	class SendMessageResponse : public Response
	{
	public:
		explicit SendMessageResponse(Stream* stream);
		virtual ~SendMessageResponse() = default;

		Types::ClientID client_id;
		uint32_t message_id;
	};
}

