#define NOMINMAX

#include "Deserializer.h"
#include "GetWaitingMessagesResponse.h"

using namespace Protocol;

GetWaitingMessagesResponse::GetWaitingMessagesResponse(Response&& response) :
	_payload(std::move(response.payload)) {}

MessageEntry GetWaitingMessagesResponse::read_next_message()
{
	const uint32_t MESSAGE_SIZE_NO_CONTENT = Common::CLIENT_ID_SIZE_BYTES + sizeof(uint32_t) + sizeof(MessageType) + sizeof(uint32_t);
	Deserializer payload(_payload.read(MESSAGE_SIZE_NO_CONTENT));

	Types::ClientID client_id = payload.read_client_id();
	auto message_id = payload.read<uint32_t>();
	auto message_type = payload.read<MessageType>();
	auto message_size = payload.read<std::uint32_t>();
	auto message_content = _payload.read(message_size);

	return MessageEntry(client_id, message_id, message_type, message_size, std::move(message_content));
}
