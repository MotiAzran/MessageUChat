#include "Serializer.h"
#include "Request.h"

using namespace Protocol;

std::string Request::serialize() const
{
	Serializer request;

	request.add(id, version, code, static_cast<uint32_t>(payload.size()), payload);

	return request.serialize();
}
