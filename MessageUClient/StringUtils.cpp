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
	if (Common::CLIENT_ID_STR_LENGTH == str.size())
	{
		auto decoded = HexWrapper::decode(str);

		return to_array<Common::CLIENT_ID_SIZE_BYTES>(decoded);
	}
	else if (Common::CLIENT_ID_SIZE_BYTES == str.size())
	{
		return to_array<Common::CLIENT_ID_SIZE_BYTES>(str);
	}
	
	throw std::exception("Invalid client id");
}

Types::PublicKey StringUtils::to_public_key(const std::string& str)
{
	if (Common::PUBLIC_KEY_SIZE != str.size())
	{
		throw std::exception("Invalid public key");
	}

	return to_array<Common::PUBLIC_KEY_SIZE>(str);
}

Types::AESKey StringUtils::to_aes_key(const std::string& str)
{
	if (Common::AES_KEY_SIZE != str.size())
	{
		throw std::exception("Invalid aes key");
	}

	return to_array<Common::AES_KEY_SIZE>(str);
}
