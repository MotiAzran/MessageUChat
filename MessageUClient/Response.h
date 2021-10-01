#pragma once

#include <cstdint>
#include <string>
#include "ProtocolCommon.h"

namespace Protocol
{
	struct Response
	{
		static const uint32_t HEADER_SIZE = sizeof(uint8_t) + sizeof(Protocol::ResponseCode) + sizeof(uint32_t);

		uint8_t version;
		Protocol::ResponseCode code;
		uint32_t payload_size;

		explicit Response(std::string&& header_data);
		virtual ~Response() = default;

	};
}
