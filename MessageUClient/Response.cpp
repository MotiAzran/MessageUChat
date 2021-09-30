#include "Deserializer.h"
#include "Response.h"

using namespace Protocol;

Response Protocol::get_response(Socket& sock)
{
	Deserializer header(sock.receive(Response::HEADER_SIZE));

	auto version = header.read<uint8_t>();
	auto code = header.read<ResponseCode>();
	auto payload_size = header.read<uint32_t>();
	
	return Response(version, code, sock.receive(payload_size));
}
