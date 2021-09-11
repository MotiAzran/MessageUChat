#pragma once

#include <string>

class Client
{
public:
	static Client client_from_file();

public:
	Client(const std::string& name, const std::string& identifier, const std::string& private_key);
	virtual ~Client() = default;

private:
	std::string _name;
	std::string _identifier;
	std::string _private_key;
};

