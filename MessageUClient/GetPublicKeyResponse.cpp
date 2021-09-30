#include "Deserializer.h"
#include "Exceptions.h"
#include "GetPublicKeyResponse.h"

using namespace Protocol;

Protocol::GetPublicKeyResponse::GetPublicKeyResponse(Response&& response)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES + Common::PUBLIC_KEY_SIZE;
	if (response.code != Protocol::ResponseCode::PublicKeySent ||
		expected_payload_size != response.payload.size())
	{
		throw ServerErrorException();
	}

	Deserializer payload(std::move(response.payload));
	client_id = payload.read_client_id();
	public_key = payload.read_public_key();
}
