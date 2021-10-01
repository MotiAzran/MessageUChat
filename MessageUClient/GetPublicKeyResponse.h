#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	struct GetPublicKeyResponse : public Response
	{
		explicit GetPublicKeyResponse(const Types::ReaderFunc& reader);
		virtual ~GetPublicKeyResponse() = default;

		Types::ClientID client_id;
		Types::PublicKey public_key;
	};
}
