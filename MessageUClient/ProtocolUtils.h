#pragma once

#include "Stream.h"
#include "ProtocolCommon.h"
#include "Types.h"

namespace Protocol
{
	namespace Utils
	{
		void send_request_header(Stream* sock, const Types::ClientID& id, const RequestCode code, const uint32_t payload_size);
	}
}