#pragma once

#include "SendMessageRequest.h"

namespace Protocol
{
	struct SendSymetricKeyRequest : public SendMessageRequest
	{
		SendSymetricKeyRequest(const Types::ClientID& id, const uint8_t version,
			const Types::ClientID& dst_client_id, std::string&& message) :
			SendMessageRequest(id, version, dst_client_id, MessageType::SendSymetricKey, std::move(message)) {}
	};
}
