#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	struct GetPublicKeyResponse
	{
		GetPublicKeyResponse(Response&& response);
		virtual ~GetPublicKeyResponse() = default;

		Types::ClientID client_id;
		Types::PublicKey public_key;
	};
}
