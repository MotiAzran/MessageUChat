#pragma once

#include <osrng.h>
#include <rsa.h>

#include <string>
#include "Types.h"

class RSAPublicWrapper
{
public:
	static const unsigned int KEYSIZE = 160;
	static const unsigned int BITS = 1024;

private:
	CryptoPP::AutoSeededRandomPool _rng;
	CryptoPP::RSA::PublicKey _publicKey;

public:
	RSAPublicWrapper(const Types::PublicKey& key);
	RSAPublicWrapper(const std::string& key);
	virtual ~RSAPublicWrapper() = default;

	RSAPublicWrapper(const RSAPublicWrapper& rsapublic) = delete;
	RSAPublicWrapper& operator=(const RSAPublicWrapper& rsapublic) = delete;

	Types::PublicKey getPublicKey() const;

	std::string encrypt(const std::string& plain);
	std::string encrypt(const char* plain, unsigned int length);
};


class RSAPrivateWrapper
{
public:
	static const unsigned int BITS = 1024;

private:
	CryptoPP::AutoSeededRandomPool _rng;
	CryptoPP::RSA::PrivateKey _privateKey;

public:
	RSAPrivateWrapper();
	RSAPrivateWrapper(const std::string& key);
	virtual ~RSAPrivateWrapper() = default;

	RSAPrivateWrapper(const RSAPrivateWrapper& rsaprivate) = delete;
	RSAPrivateWrapper& operator=(const RSAPrivateWrapper& rsaprivate) = delete;

	std::string getPrivateKey() const;
	Types::PublicKey getPublicKey() const;

	std::string decrypt(const std::string& cipher);
	std::string decrypt(const char* plain, unsigned int length);
};
