#pragma once

#include <cstdint>
#include <string>
#include "ProtocolCommon.h"
#include "Socket.h"

namespace Protocol
{
	struct Response
	{
		static const uint32_t HEADER_SIZE = sizeof(uint8_t) + sizeof(Protocol::ResponseCode) + sizeof(uint32_t);

		uint8_t version;
		ResponseCode code;
		std::string payload;

		explicit Response(const uint8_t version, const ResponseCode code, std::string&& payload) :
			version(version), code(code), payload(std::move(payload)) {}
		virtual ~Response() = default;
	};

	Response get_response(Socket& sock);
}
