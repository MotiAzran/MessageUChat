#include <algorithm>
#include "StringUtils.h"

bool StringUtils::is_number(const std::string& str)
{
	if (str.empty())
	{
		return false;
	}

	return std::all_of(str.cbegin(), str.cend(), std::isdigit);
}

std::vector<std::string> StringUtils::split(const std::string& str, const char sep)
{
	std::vector<std::string> splited;

	int last_idx = 0;
	for (auto idx = str.find(sep);
		str.npos != idx;
		idx = str.find(sep, idx + 1))
	{
		splited.push_back(str.substr(last_idx, (idx - last_idx)));
		last_idx = idx + 1;
	}

	splited.push_back(str.substr(last_idx));

	return splited;
}
