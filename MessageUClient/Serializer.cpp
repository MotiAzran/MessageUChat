#include "Serializer.h"

void Serializer::add(const std::string& str)
{
	_data += str;
}

void Serializer::add(const Types::ClientID& id)
{
	add(StringUtils::to_string(id));
}
