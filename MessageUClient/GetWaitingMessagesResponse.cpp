#define NOMINMAX

#include "Deserializer.h"
#include "GetWaitingMessagesResponse.h"

using namespace Protocol;

GetWaitingMessagesResponse::GetWaitingMessagesResponse(Stream* stream) :
	Response(stream),
	_stream(stream),
	_size_left_to_read(payload_size) {}

GetWaitingMessagesResponse::MessageEntry GetWaitingMessagesResponse::read_next_message()
{
	Deserializer payload(_stream->read(MessageEntry::SIZE_NO_CONTENT));
	_size_left_to_read -= MessageEntry::SIZE_NO_CONTENT;

	Types::ClientID client_id = payload.read_client_id();
	auto message_id = payload.read<uint32_t>();
	auto message_type = payload.read<MessageType>();
	auto message_size = payload.read<std::uint32_t>();
	auto message_content = _stream->read(message_size);
	_size_left_to_read -= message_size;

	return MessageEntry(client_id, message_id, message_type, message_size, std::move(message_content));
}
