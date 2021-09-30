#include "Serializer.h"
#include "Request.h"

using namespace Protocol;

std::string Request::serialize() const
{
	Serializer request;

	request.add(id, version, code, payload_size, payload);

	return request.serialize();
}
