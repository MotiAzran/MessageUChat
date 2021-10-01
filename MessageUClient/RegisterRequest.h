#pragma once

#include "Request.h"

namespace Protocol
{
	struct RegisterRequest : public Request
	{
		RegisterRequest(const Types::ClientID& id, const uint8_t version,
			const std::string& name, const Types::PublicKey& public_key) :
			Request(id, version, RequestCode::Register, _serialize_payload(name, public_key)) {}

	private:
		static std::string _serialize_payload(const std::string& name, const Types::PublicKey& public_key);
	};
}
