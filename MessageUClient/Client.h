#pragma once

#include <cstdint>
#include <string>
#include <array>
#include "FileStream.h"
#include "RSAWrapper.h"
#include "Types.h"

class Client
{
public:
	static Client* client_from_file(FileStream& info_file);
	static Client* client_from_file(FileStream&& info_file);

public:
	Client(const std::string& name, const Types::ClientID& identifier, const std::string& private_key);
	virtual ~Client() = default;

private:
	std::string _name;
	Types::ClientID _identifier;
	RSAPrivateWrapper _private_key;
};

