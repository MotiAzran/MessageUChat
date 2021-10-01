#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "RegisterResponse.h"

using namespace Protocol;

RegisterResponse::RegisterResponse(const Types::ReaderFunc& reader) :
	Response(reader)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES;
	if (code != ResponseCode::RegisterSuccess ||
		expected_payload_size != payload_size)
	{
		throw ServerErrorException();
	}

	Deserializer payload(reader(payload_size));
	client_id = payload.read_client_id();
}
