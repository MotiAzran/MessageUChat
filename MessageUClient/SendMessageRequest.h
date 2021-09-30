#pragma once

#include "Request.h"

namespace Protocol
{
	struct SendMessageRequest : public Request
	{
		SendMessageRequest(const Types::ClientID& id, const uint8_t version, const Types::ClientID& dst_client_id,
			const Protocol::MessageType message_type, std::string&& message_content) :
			Request(id, version, RequestCode::SendMessageToUser,
				_serialize_payload(dst_client_id, message_type, std::move(message_content))) {}

	private:
		static std::string _serialize_payload(const Types::ClientID& dst_client_id,
			const Protocol::MessageType message_type, std::string&& message_content);
	};
}
