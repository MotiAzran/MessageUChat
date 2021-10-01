#pragma once

#include "SendMessageRequest.h"

namespace Protocol
{
	struct RequestSymetricKeyRequest : public SendMessageRequest
	{
		RequestSymetricKeyRequest(const Types::ClientID& id, const uint8_t version, const Types::ClientID& dst_client_id) :
			SendMessageRequest(id, version, dst_client_id, MessageType::SymetricKeyRequest) {}
	};
}
