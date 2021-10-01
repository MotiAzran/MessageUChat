#pragma once

#include "Request.h"

namespace Protocol
{
	struct GetClientsListRequest : public Request
	{
		GetClientsListRequest(const Types::ClientID& id, const uint8_t version) :
			Request(id, version, RequestCode::ClientsList) {}
	};
}
