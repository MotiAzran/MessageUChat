#pragma once

#include "AESWrapper.h"
#include "Stream.h"

class AESStream : public Stream
{
public: 
	explicit AESStream(Stream* stream);
	AESStream(const std::string& key, Stream* stream);
	virtual ~AESStream();

	virtual std::string read(const uint32_t size);
	virtual void write(const std::string& buf);

private:
	AESWrapper _aes;
	Stream* _stream;
};
