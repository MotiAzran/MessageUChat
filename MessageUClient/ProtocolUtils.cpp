#include "Serializer.h"
#include "Deserializer.h"
#include "ProtocolUtils.h"

void Protocol::Utils::send_request_header(Stream* stream, const Types::ClientID& id, const RequestCode code, const uint32_t payload_size)
{
	Serializer header;

	header.add(id, Common::VERSION, code, payload_size);

	stream->write(header.serialize());
}

Protocol::ResponseHeader Protocol::Utils::get_response_header(Stream* stream)
{
	const auto size_of_header = sizeof(uint8_t) + sizeof(ResponseCode) + sizeof(uint32_t);
	Deserializer header(stream->read(size_of_header));

	auto version = header.read<uint8_t>();
	auto code = header.read<ResponseCode>();
	auto payload_size = header.read<uint32_t>();

	return ResponseHeader(version, code, payload_size);
}
