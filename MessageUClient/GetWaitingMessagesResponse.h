#pragma once

#include "Response.h"
#include "ProtocolCommon.h"
#include "Types.h"

namespace Protocol
{
	class GetWaitingMessagesResponse : public Response
	{
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
