#pragma once

#include "SendMessageRequest.h"

namespace Protocol
{
	struct SendTextMessageRequest : public SendMessageRequest
	{
		SendTextMessageRequest(const Types::ClientID& id, const uint8_t version,
			const Types::ClientID& dst_client_id, std::string&& message) :
			SendMessageRequest(id, version, dst_client_id, MessageType::Text, std::move(message)) {}
	};
}
