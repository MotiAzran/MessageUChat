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

	template <typename T>
	T to_pod(const std::string& str)
	{
		T val;
		CopyMemory(&val, str.data(), sizeof(val));

		return val;
	}

	template <typename T, typename = std::enable_if_t<std::is_pod_v<T>>>
	std::string to_string(const T& val)
	{
		std::string str(sizeof(val), '\0');
		CopyMemory(str.data(), &val, sizeof(val));

		return str;
	}

	std::string to_string(const Types::ClientID& id);
}
