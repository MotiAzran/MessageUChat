#pragma once

#include <cstdint>
#include <string>
#include "ProtocolCommon.h"

namespace Protocol
{
	struct Response
	{
		uint8_t version;
		Protocol::ResponseCode code;
		uint32_t payload_size;

		explicit Response(const Types::ReaderFunc& reader);
		virtual ~Response() = default;

	};
}
