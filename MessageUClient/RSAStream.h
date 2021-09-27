#pragma once

#include "RSAWrapper.h"
#include "Stream.h"

class RSAStream : public Stream
{
public:
	explicit RSAStream(const std::string& private_key, const std::string& public_key, Stream* stream);
	virtual ~RSAStream();

	virtual std::string read(const uint32_t size) override;
	virtual void write(const std::string& buf) override;
	virtual void write(const char* buf, const uint32_t length) override;

private:
	RSAPrivateWrapper _rsapriv;
	RSAPublicWrapper _rsapub;
	Stream* _stream;
};
