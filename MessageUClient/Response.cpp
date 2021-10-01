#include "Deserializer.h"
#include "Response.h"

using namespace Protocol;

Response::Response(const Types::ReaderFunc& reader)
{
	const uint32_t HEADER_SIZE = sizeof(uint8_t) + sizeof(Protocol::ResponseCode) + sizeof(uint32_t);
	Deserializer header(reader(HEADER_SIZE));

	version = header.read<uint8_t>();
	code = header.read<Protocol::ResponseCode>();
	payload_size = header.read<uint32_t>();
}
