#pragma once

#include <exception>

class ExitException : public std::exception {};

class ServerErrorException : public std::exception
{
public:
	ServerErrorException() :
		std::exception("server responded with an error") {}
};
