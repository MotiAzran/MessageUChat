#include <algorithm>
#include "HexWrapper.h"
#include "Common.h"
#include "StringUtils.h"

bool StringUtils::is_number(const std::string& str)
{
	if (str.empty())
	{
		return false;
	}

	return std::all_of(str.begin(), str.end(), [](const char c) { return '0' <= c && c <= '9'; });
}

Types::ClientID StringUtils::to_client_id(const std::string& str)
{
	if (Common::CLIENT_IDENTIFIER_STR_LENGTH == str.size())
	{
		auto decoded = HexWrapper::decode(str);

		return Types::ClientID(to_array<Common::CLIENT_IDENTIFIER_SIZE_BYTES>(decoded));
	}
	else if (Common::CLIENT_IDENTIFIER_SIZE_BYTES == str.size())
	{
		return Types::ClientID(to_array<Common::CLIENT_IDENTIFIER_SIZE_BYTES>(str));
	}
	
	throw std::exception("Invalid client id");
}

std::string StringUtils::to_string(const Types::ClientID& id)
{
	std::string id_str(id.size(), 0);
	CopyMemory(id_str.data(), id.data(), id.size());

	return id_str;
}
