#include "Deserializer.h"
#include "Response.h"

using namespace Protocol;

Response::Response(Stream* stream)
{
	Deserializer header(stream->read(HEADER_SIZE));

	version = header.read<uint8_t>();
	code = header.read<Protocol::ResponseCode>();
	payload_size = header.read<uint32_t>();
}
