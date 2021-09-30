#pragma once

#include "Request.h"
#include "StringUtils.h"

namespace Protocol
{
	struct GetPublicKeyRequest : public Request
	{
		GetPublicKeyRequest(const Types::ClientID& id, const uint8_t version, const Types::ClientID& other_client_id) :
			Request(id, version, RequestCode::PublicKeyRequest, StringUtils::to_string(other_client_id)) {}
	};
}
