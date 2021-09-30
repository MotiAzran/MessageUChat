#include "Deserializer.h"
#include "Common.h"
#include "Exceptions.h"
#include "ClientsListResponse.h"

using namespace Protocol;

ClientsListResponse::ClientsListResponse(Response&& response) :
	_payload(std::move(response.payload))
{
	if (ResponseCode::ClientsListSent != response.code)
	{
		throw ServerErrorException();
	}
}

ClientEntry ClientsListResponse::get_next_entry()
{
	Deserializer entry(_payload.read(ClientEntry::SIZE));

	Types::ClientID id = entry.read_client_id();
	auto name = entry.read(Common::MAX_CLIENT_NAME_LENGTH);
	name.erase(name.find('\0'));

	return ClientEntry(id, name);
}
