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

	const auto MAX_PORT = 0xffff;

	const auto MAX_CLIENT_NAME_LENGTH = 255;
	const auto PUBLIC_KEY_LENGTH = 160;
	const auto CLIENT_IDENTIFIER_STR_LENGTH = 32;
	const auto CLIENT_IDENTIFIER_SIZE_BYTES = 16;

	static const std::filesystem::path SERVER_INFO_PATH = "server.info";
	static const std::filesystem::path CLIENT_INFO_FILE_PATH = "me.info";
}
