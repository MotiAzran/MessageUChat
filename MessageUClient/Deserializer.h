#pragma once

#include <cstdint>
#include <type_traits>
#include <string>
#include "StringUtils.h"
#include "Types.h"
#include "Common.h"

class Deserializer
{
public:
	explicit Deserializer(std::string&& data);
	virtual ~Deserializer() = default;

	std::string read(const uint32_t size);

	template <typename Val, typename = std::enable_if_t<std::is_pod_v<Val>>>
	Val read()
	{
		return StringUtils::to_pod<Val>(read(sizeof(Val)));
	}

	Types::ClientID read_client_id();
	Types::PublicKey read_public_key();

private:
	std::string _data;
};
