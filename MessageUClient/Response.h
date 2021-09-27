#pragma once

#include <cstdint>
#include "Stream.h"
#include "ProtocolCommon.h"

namespace Protocol
{
	class Response
	{
	public:
		const uint32_t HEADER_SIZE = sizeof(uint8_t) + sizeof(Protocol::ResponseCode) + sizeof(uint32_t);

	public:
		explicit Response(Stream* stream);
		virtual ~Response() = default;

		uint8_t version;
		Protocol::ResponseCode code;
		uint32_t payload_size;
	};
}
