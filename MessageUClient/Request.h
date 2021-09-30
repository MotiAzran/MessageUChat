#pragma once

#include <string>
#include "Types.h"
#include "ProtocolCommon.h"

namespace Protocol
{
	struct Request
	{
		Types::ClientID id;
		uint8_t version;
		RequestCode code;
		uint32_t payload_size;
		std::string payload;

		Request(const Types::ClientID& id, const uint8_t version, const RequestCode code, std::string&& payload) :
			id(id), version(version), code(code), payload_size(payload.size()),
			payload(std::move(payload)) {}

		std::string serialize() const;
	};
}