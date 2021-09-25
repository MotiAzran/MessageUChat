#include "AESWrapper.h"

#include <modes.h>
#include <aes.h>
#include <filters.h>

#include <stdexcept>
#include <immintrin.h>	// _rdrand32_step


std::string AESWrapper::GenerateKey(unsigned int length)
{
	std::string key(length, '\0');
	for (size_t i = 0; i < length; i += sizeof(unsigned int))
		_rdrand32_step(reinterpret_cast<unsigned int*>(&key[i]));
	
	return key;
}

AESWrapper::AESWrapper() :
	_key(GenerateKey()) {}

AESWrapper::AESWrapper(const std::string& key) :
	_key(key)
{
	if (key.size() != DEFAULT_KEYLENGTH)
		throw std::length_error("key length must be 16 bytes");
}

std::string AESWrapper::encrypt(const std::string& plain)
{
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

	CryptoPP::AES::Encryption aesEncryption(reinterpret_cast<CryptoPP::byte*>(_key.data()), _key.size());
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

	std::string cipher;
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipher));
	stfEncryptor.Put(reinterpret_cast<const CryptoPP::byte*>(plain.data()), plain.size());
	stfEncryptor.MessageEnd();

	return cipher;
}


std::string AESWrapper::decrypt(const std::string& cipher)
{
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

	CryptoPP::AES::Decryption aesDecryption(reinterpret_cast<CryptoPP::byte*>(_key.data()), _key.size());
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

	std::string decrypted;
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decrypted));
	stfDecryptor.Put(reinterpret_cast<const CryptoPP::byte*>(cipher.data()), cipher.size());
	stfDecryptor.MessageEnd();

	return decrypted;
}
