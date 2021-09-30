#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "RegisterResponse.h"

using namespace Protocol;

RegisterResponse::RegisterResponse(Stream* stream) : 
	Response(stream)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES;
	if (code != ResponseCode::RegisterSuccess ||
		expected_payload_size != payload_size)
	{
		throw ServerErrorException();
	}

	Deserializer payload(stream->receive(expected_payload_size));
	client_id = payload.read_client_id();
}
