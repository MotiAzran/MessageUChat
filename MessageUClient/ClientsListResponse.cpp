#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "ClientsListResponse.h"

using namespace Protocol;

ClientsListResponse::ClientsListResponse(const Types::ReaderFunc& reader) :
	Response(reader(Response::HEADER_SIZE)),
	_reader(reader),
	_remaining_entries(payload_size / ClientEntry::SIZE)
{
	if (ResponseCode::ClientsListSent != code)
	{
		throw ServerErrorException();
	}
}

ClientEntry ClientsListResponse::get_next_entry()
{
	Deserializer entry(_reader(ClientEntry::SIZE));
	--_remaining_entries;

	Types::ClientID id = entry.read_client_id();
	auto name = entry.read(Common::MAX_CLIENT_NAME_LENGTH);
	name.erase(name.find('\0'));

	return ClientEntry(id, name);
}
