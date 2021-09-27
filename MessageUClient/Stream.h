#pragma once

#include <cstdint>
#include <string>

class Stream
{
public:
	virtual std::string read(const uint32_t size) = 0;
	virtual void write(const std::string& buf) = 0;
	virtual void write(const char* buf, const uint32_t length) = 0;
};
