#pragma once

#include <vector>
#include <string>

namespace StringUtils
{
	bool is_number(const std::string& str);
	std::vector<std::string> split(const std::string& str, const char sep);
}
