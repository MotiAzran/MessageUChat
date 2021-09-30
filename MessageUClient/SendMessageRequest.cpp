#include "Serializer.h"
#include "SendMessageRequest.h"

std::string Protocol::SendMessageRequest::_serialize_payload(const Types::ClientID& other_client_id, const Protocol::MessageType message_type, std::string&& message_content)
{
	Serializer payload;

	payload.add(other_client_id, message_type, static_cast<uint32_t>(message_content.size()), message_content);

	return payload.serialize();
}
