#include <algorithm>
#include "ClientsList.h"

void ClientsList::insert(const Types::ClientID& id, const std::string& name,
	const Types::PublicKey& public_key, const Types::AESKey& aes_key)
{
	_clients[MapKey(id, name)] = MapValue(public_key, aes_key);
}

void ClientsList::insert(const Types::ClientID& id, const std::string& name)
{
	Types::PublicKey empty_public_key;
	empty_public_key.fill(0);

	Types::AESKey empty_aes_key;
	empty_aes_key.fill(0);

	insert(id, name, empty_public_key, empty_aes_key);
}
