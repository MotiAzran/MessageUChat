#include "Deserializer.h"
#include "Exceptions.h"
#include "GetPublicKeyResponse.h"

using namespace Protocol;

GetPublicKeyResponse::GetPublicKeyResponse(Stream* stream) :
	Response(stream)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES + Common::PUBLIC_KEY_SIZE;
	if (code != Protocol::ResponseCode::PublicKeySent ||
		expected_payload_size != payload_size)
	{
		throw ServerErrorException();
	}

	Deserializer payload(stream->receive(expected_payload_size));
	client_id = payload.read_client_id();
	public_key = payload.read_public_key();
}
