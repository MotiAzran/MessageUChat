#include "Serializer.h"
#include "RegisterRequest.h"

std::string Protocol::RegisterRequest::_serialize_payload(const std::string& name, const Types::PublicKey& public_key)
{
	Serializer payload;

	auto sized_name = name;
	sized_name.resize(Common::MAX_CLIENT_NAME_LENGTH);

	payload.add(sized_name, public_key);

	return payload.serialize();
}
