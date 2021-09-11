#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
#include <string>

#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

namespace Common
{
	static const uint8_t VERSION = 2;
	static const uint32_t CHUNK_SIZE = 64 * 1024;
}
