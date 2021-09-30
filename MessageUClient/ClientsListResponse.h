#pragma once

#include "Response.h"
#include "ProtocolCommon.h"
#include "Deserializer.h"

namespace Protocol
{
	class ClientsListResponse
	{
	public:
		explicit ClientsListResponse(Response&& response);
		virtual ~ClientsListResponse() = default;

		ClientEntry get_next_entry();
		bool is_done() const { return 0 == _payload.size(); }

	private:
		Deserializer _payload;
	};
}
