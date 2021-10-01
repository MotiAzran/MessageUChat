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
	static const uint8_t VERSION = 1;

	static const auto MAX_CLIENT_NAME_LENGTH = 255;
	static const auto PUBLIC_KEY_SIZE = 160;
	static const auto AES_KEY_SIZE = 16;
	static const auto CLIENT_ID_STR_LENGTH = 32;
	static const auto CLIENT_ID_SIZE_BYTES = 16;

	static const std::filesystem::path SERVER_INFO_PATH = "server.info";
	static const std::filesystem::path CLIENT_INFO_FILE_PATH = "me.info";
}
