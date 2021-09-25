#include "AESStream.h"

AESStream::AESStream(Stream* stream) :
	AESStream(AESWrapper::GenerateKey(), stream) {}

AESStream::AESStream(const std::string& key, Stream* stream) :
	_aes(key),
	_stream(stream)
{
	if (nullptr == stream)
	{
		throw std::exception("Got invalid stream");
	}
}

AESStream::~AESStream()
{
	try
	{
		delete _stream;
	} catch(...) {}
}

std::string AESStream::read(const uint32_t size)
{
	return _aes.decrypt(_stream->read(size));
}

void AESStream::write(const std::string& buf)
{
	_stream->write(_aes.encrypt(buf));
}
