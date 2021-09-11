#pragma once

#include <string>
#include <base64.h>


namespace Base64Wrapper
{
	std::string encode(const std::string& str);
	std::string decode(const std::string& str);
};
