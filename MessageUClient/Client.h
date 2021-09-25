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

	std::string get_name() const { return _name; }
	Types::ClientID get_id() const { return _identifier; }
	std::string get_private_key() const { return _private_key.getPrivateKey(); }
	std::string get_public_key() const { return _private_key.getPublicKey(); }

private:
	std::string _name;
	Types::ClientID _identifier;
	RSAPrivateWrapper _private_key;
};

