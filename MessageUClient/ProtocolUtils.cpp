#include "Serializer.h"
#include "Deserializer.h"
#include "ProtocolUtils.h"

void Protocol::Utils::send_request_header(Stream* stream, const Types::ClientID& id, const RequestCode code, const uint32_t payload_size)
{
	Serializer header;

	header.add(id, Common::VERSION, code, payload_size);

	stream->write(header.serialize());
}
