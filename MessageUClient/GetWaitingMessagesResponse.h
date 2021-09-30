#pragma once

#include "Response.h"
#include "ProtocolCommon.h"
#include "Deserializer.h"
#include "Types.h"

namespace Protocol
{
	class GetWaitingMessagesResponse
	{
	public:
		explicit GetWaitingMessagesResponse(Response&& response);
		virtual ~GetWaitingMessagesResponse() = default;

		MessageEntry read_next_message();
		bool is_done() const { return 0 == _payload.size(); }

	private:
		Deserializer _payload;
	};
}
