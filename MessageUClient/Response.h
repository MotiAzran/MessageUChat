#pragma once

#include <cstdint>
#include "Stream.h"
#include "ProtocolCommon.h"

namespace Protocol
{
	struct Response
	{
		explicit Response(Stream* stream);
		virtual ~Response() = default;

		uint8_t version;
		Protocol::ResponseCode code;
		uint32_t payload_size;
	};
}
