#pragma once

#include <string>


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
	virtual ~AESWrapper() = default;

	std::string getKey() const { return _key; }

	std::string encrypt(const std::string& plain);
	std::string decrypt(const std::string& cipher);
};