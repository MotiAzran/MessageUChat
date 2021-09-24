#include "Serializer.h"
#include "ProtocolUtils.h"

void Protocol::Utils::send_request_header(SocketStream& sock, const Types::ClientID& id, const RequestCode code, const uint32_t payload_size)
{
	Serializer header;

	header.add(id, Common::VERSION, code, payload_size);

	sock.write(header.serialize());
}

Protocol::ResponseHeader Protocol::Utils::get_response_header(SocketStream& sock)
{
	auto version = sock.read_pod<uint8_t>();
	auto code = sock.read_pod<ResponseCode>();
	auto payload_size = sock.read_pod<uint32_t>();

	return ResponseHeader(version, code, payload_size);
}
