#pragma once

#include <osrng.h>
#include <rsa.h>

#include <string>

class RSAPublicWrapper
{
public:
	static const unsigned int KEYSIZE = 160;
	static const unsigned int BITS = 1024;

private:
	CryptoPP::AutoSeededRandomPool _rng;
	CryptoPP::RSA::PublicKey _publicKey;

public:
	RSAPublicWrapper(const std::string& key);
	virtual ~RSAPublicWrapper() = default;

	RSAPublicWrapper(const RSAPublicWrapper& rsapublic) = delete;
	RSAPublicWrapper& operator=(const RSAPublicWrapper& rsapublic) = delete;

	std::string getPublicKey() const;

	std::string encrypt(const std::string& plain);
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
	std::string getPublicKey() const;

	std::string decrypt(const std::string& cipher);
};
