#pragma once

#include "Response.h"
#include "Types.h"

namespace Protocol
{
	class GetPublicKeyResponse : public Response
	{
	public:
		explicit GetPublicKeyResponse(Stream* stream);
		virtual ~GetPublicKeyResponse() = default;

		Types::ClientID client_id;
		Types::PublicKey public_key;
	};
}
