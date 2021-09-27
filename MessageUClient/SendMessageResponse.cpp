#include "Deserializer.h"
#include "Exceptions.h"
#include "SendMessageResponse.h"

using namespace Protocol;

SendMessageResponse::SendMessageResponse(Stream* stream) :
	Response(stream)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES + sizeof(uint32_t);
	if (expected_payload_size != payload_size ||
		ResponseCode::MessageSentToUser != code)
	{
		throw ServerErrorException();
	}
	
	Deserializer payload(stream->read(expected_payload_size));
	client_id = payload.read_client_id();
	message_id = payload.read<uint32_t>();
}
