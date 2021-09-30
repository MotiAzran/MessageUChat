#pragma once

#include "Request.h"

namespace Protocol
{
	struct GetWaitingMessagesRequest : public Request
	{
		GetWaitingMessagesRequest(const Types::ClientID& id, const uint8_t version) :
			Request(id, version, RequestCode::WaitingMessagesRequest, std::string()) {}
	};
}
