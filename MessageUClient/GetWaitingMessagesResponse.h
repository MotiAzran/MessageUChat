#pragma once

#include "Response.h"
#include "ProtocolCommon.h"
#include "Deserializer.h"
#include "Types.h"

namespace Protocol
{
	class GetWaitingMessagesResponse : public Response
	{
	public:
		explicit GetWaitingMessagesResponse(const Types::ReaderFunc& reader);
		virtual ~GetWaitingMessagesResponse() = default;

		MessageEntry read_next_message();
		bool is_done() const { return 0 == _size_left_to_read; }

	private:
		Types::ReaderFunc _reader;
		uint32_t _size_left_to_read;
	};
}
