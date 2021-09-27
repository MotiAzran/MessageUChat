#pragma once

#include <utility>
#include <array>
#include "Common.h"
#include "IPAddress.h"

namespace Types
{
	using ClientID = std::array<uint8_t, Common::CLIENT_ID_SIZE_BYTES>;
	using PublicKey = std::array<uint8_t, Common::PUBLIC_KEY_SIZE>;
	using AESKey = std::array<uint8_t, Common::AES_KEY_SIZE>;
	using Port = uint16_t;
	using Host = std::pair<IPAddress, Port>;
}
