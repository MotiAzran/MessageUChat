#pragma once

#include <string>
#include <hex.h>

namespace HexWrapper
{
	std::string encode(const std::string& str);
	std::string decode(const std::string& str);
};

