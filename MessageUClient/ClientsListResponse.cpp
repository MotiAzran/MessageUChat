#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "ClientsListResponse.h"

using namespace Protocol;

ClientsListResponse::ClientsListResponse(Stream* stream) :
	Response(stream),
	_stream(stream),
	_remaining_entries(payload_size / ClientsListResponse::ClientEntry::SIZE)
{
	if (ResponseCode::ClientsListSent != code)
	{
		throw ServerErrorException();
	}
}

ClientsListResponse::ClientEntry ClientsListResponse::get_next_entry()
{
	if (0 == _remaining_entries)
	{
		throw std::out_of_range("No more clients entries");
	}

	Deserializer entry(_stream->receive(ClientEntry::SIZE));
	--_remaining_entries;

	Types::ClientID id = entry.read_client_id();
	auto name = entry.read(Common::MAX_CLIENT_NAME_LENGTH);
	name.erase(name.find('\0'));

	return ClientEntry(id, name);
}
