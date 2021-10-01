#pragma once

#include <functional>
#include "Response.h"
#include "ProtocolCommon.h"
#include "Types.h"

namespace Protocol
{
	class ClientsListResponse : public Response
	{
	public:
		explicit ClientsListResponse(const Types::ReaderFunc& reader);
		virtual ~ClientsListResponse() = default;

		ClientEntry get_next_entry();
		bool is_done() const { return 0 == _remaining_entries; }

	private:
		Types::ReaderFunc _reader;
		uint32_t _remaining_entries;
	};
}
