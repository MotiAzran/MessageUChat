#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "RegisterResponse.h"

using namespace Protocol;

RegisterResponse::RegisterResponse(Response&& response)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES;
	if (response.code != ResponseCode::RegisterSuccess ||
		expected_payload_size != response.payload.size())
	{
		throw ServerErrorException();
	}

	Deserializer payload(std::move(response.payload));
	client_id = payload.read_client_id();
}
