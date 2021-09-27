#pragma once

#include <string>
#include "Types.h"


class AESWrapper
{
public:
	static const unsigned int DEFAULT_KEYLENGTH = 16;
private:
	std::string _key;
	AESWrapper(const AESWrapper& aes);

public:
	static std::string GenerateKey(unsigned int length=DEFAULT_KEYLENGTH);

	AESWrapper();
	AESWrapper(const std::string& key);
	AESWrapper(const Types::AESKey& key);
	virtual ~AESWrapper() = default;

	std::string getKey() const { return _key; }

	std::string encrypt(const std::string& plain);
	std::string encrypt(const char* plain, const uint32_t length);
	std::string decrypt(const std::string& cipher);
	std::string decrypt(const char* plain, const uint32_t length);
};