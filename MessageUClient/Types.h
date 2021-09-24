#pragma once

#include <utility>
#include <array>
#include "Common.h"
#include "IPAddress.h"

namespace Types
{
	using ClientID = std::array<uint8_t, Common::CLIENT_IDENTIFIER_SIZE_BYTES>;

	using Port = uint16_t;
	using Host = std::pair<IPAddress, Port>;
}
