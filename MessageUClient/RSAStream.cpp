#include "RSAStream.h"


RSAStream::RSAStream(const std::string& private_key, const std::string& public_key, Stream* stream) :
	_rsapriv(private_key),
	_rsapub(public_key),
	_stream(stream)
{
	if (nullptr == stream)
	{
		throw std::exception("Got invalid stream");
	}
}

RSAStream::~RSAStream()
{
	try
	{
		delete _stream;
	} catch(...) {}
}

std::string RSAStream::read(const uint32_t size)
{
	return _rsapriv.decrypt(_stream->read(size));
}

void RSAStream::write(const std::string& buf)
{
	write(buf.data(), buf.size());
}

void RSAStream::write(const char* buf, const uint32_t length)
{
	_stream->write(_rsapub.encrypt(buf, length));
}
