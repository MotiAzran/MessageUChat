#pragma once

#include <vector>
#include <string>
#include "Types.h"


namespace StringUtils
{
	bool is_number(const std::string& str);

	template <size_t N>
	std::array<uint8_t, N> to_array(const std::string& str)
	{
		std::array<uint8_t, N> arr;
		std::copy(str.begin(), str.end(), arr.begin());

		return arr;
	}

	Types::ClientID to_client_id(const std::string& str);
	Types::PublicKey to_public_key(const std::string& str);
	Types::AESKey to_aes_key(const std::string& str);

	template <typename Val>
	Val to_pod(const std::string& str)
	{
		Val val;
		CopyMemory(&val, str.data(), sizeof(val));

		return val;
	}

	template <typename Val, typename = std::enable_if_t<std::is_pod_v<Val>>>
	std::string to_string(const Val& val)
	{
		std::string str(sizeof(val), '\0');
		CopyMemory(str.data(), &val, sizeof(val));

		return str;
	}

	template <typename T, size_t N>
	std::string to_string(const std::array<T, N>& arr)
	{
		std::string arr_str(arr.size(), 0);
		CopyMemory(arr_str.data(), arr.data(), arr.size());

		return arr_str;
	}
}
