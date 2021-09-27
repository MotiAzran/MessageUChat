#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	class GetWaitingMessagesResponse : public Response
	{
	public:
		struct MessageEntry
		{
			static const uint32_t SIZE_NO_CONTENT = Common::CLIENT_ID_SIZE_BYTES + sizeof(uint32_t) + sizeof(MessageType) + sizeof(uint32_t);

			Types::ClientID client_id;
			uint32_t message_id;
			MessageType message_type;
			uint32_t message_size;
			std::string message_content;

			MessageEntry(const Types::ClientID& client_id, const uint32_t message_id, const MessageType message_type,
				const uint32_t message_size, std::string&& message_content) :
				client_id(client_id), message_id(message_id), message_type(message_type),
				message_size(message_size), message_content(std::move(message_content)) {}
		};

	public:
		explicit GetWaitingMessagesResponse(Stream* stream);
		virtual ~GetWaitingMessagesResponse() = default;

		MessageEntry read_next_message();
		bool is_done() const { return 0 == _size_left_to_read; }

	private:
		Stream* _stream;
		uint32_t _size_left_to_read;
	};
}
