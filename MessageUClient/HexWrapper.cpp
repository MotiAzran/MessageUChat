#include "HexWrapper.h"

std::string HexWrapper::encode(const std::string& str)
{
	std::string encoded;
	CryptoPP::StringSource ss(str, true,
		new CryptoPP::HexEncoder(
			new CryptoPP::StringSink(encoded)
		) // HexEncoder
	); // StringSource

	return encoded;
}

std::string HexWrapper::decode(const std::string& str)
{
	std::string decoded;
	CryptoPP::StringSource ss(str, true,
		new CryptoPP::HexDecoder(
			new CryptoPP::StringSink(decoded)
		) // HexDecoder
	); // StringSource

	return decoded;
}
