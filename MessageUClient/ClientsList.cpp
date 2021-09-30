#include <algorithm>
#include "ClientsList.h"

void ClientsList::insert(const Types::ClientID& id, const std::string& name,
	const Types::PublicKey& rsapub, const Types::AESKey& aes)
{
	_clients[MapKey(id, name)] = MapValue(rsapub, aes);
}

void ClientsList::insert(const Types::ClientID& id, const std::string& name)
{
	Types::PublicKey empty_public_key;
	empty_public_key.fill(0);

	Types::AESKey empty_aes_key;
	empty_aes_key.fill(0);

	insert(id, name, empty_public_key, empty_aes_key);
}
