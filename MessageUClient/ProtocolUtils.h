#pragma once

#include "SocketStream.h"
#include "ProtocolCommon.h"
#include "Types.h"

namespace Protocol
{
	namespace Utils
	{
		void send_request_header(SocketStream& sock, const Types::ClientID& id, const RequestCode code, const uint32_t payload_size);
		ResponseHeader get_response_header(SocketStream& sock);
	}
}