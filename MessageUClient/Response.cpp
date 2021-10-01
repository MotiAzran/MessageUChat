#include "Deserializer.h"
#include "Response.h"

using namespace Protocol;

Response::Response(std::string&& header_data)
{
	Deserializer header(std::move(header_data));

	version = header.read<uint8_t>();
	code = header.read<Protocol::ResponseCode>();
	payload_size = header.read<uint32_t>();
}
