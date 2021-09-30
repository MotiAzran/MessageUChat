#include "Deserializer.h"
#include "Exceptions.h"
#include "SendMessageResponse.h"

using namespace Protocol;

SendMessageResponse::SendMessageResponse(Response&& response)
{
	const auto expected_payload_size = Common::CLIENT_ID_SIZE_BYTES + sizeof(uint32_t);
	if (expected_payload_size != response.payload.size() ||
		ResponseCode::MessageSentToUser != response.code)
	{
		throw ServerErrorException();
	}
	
	Deserializer payload(std::move(response.payload));
	client_id = payload.read_client_id();
	message_id = payload.read<uint32_t>();
}
