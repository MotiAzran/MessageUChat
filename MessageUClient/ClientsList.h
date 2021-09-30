#pragma once

#include <tuple>
#include <map>
#include <string>
#include "Types.h"
#include "AESWrapper.h"
#include "RSAWrapper.h"

class ClientsList
{
public:
	using MapKey = std::tuple<Types::ClientID, std::string>;
	using MapValue = std::tuple<Types::PublicKey, Types::AESKey>;

	struct ClientField
	{
		Types::ClientID id;
		std::string name;
		RSAPublicWrapper rsapub;
		AESWrapper aes;

		ClientField(const Types::ClientID& id, const std::string& name,
			const Types::PublicKey& rsapub, const Types::AESKey& aes) :
			id(id), name(name), rsapub(rsapub), aes(aes) {}

		ClientField(const MapKey& key, const MapValue& value) :
			id(std::get<Types::ClientID>(key)), name(std::get<std::string>(key)),
			rsapub(std::get<Types::PublicKey>(value)), aes(std::get<Types::AESKey>(value)) {}
	};

public:
	ClientsList() = default;
	virtual ~ClientsList() = default;

	void insert(const Types::ClientID& id, const std::string& name,
		const Types::PublicKey& rsapub, const Types::AESKey& aes);
	void insert(const Types::ClientID& id, const std::string& name);
	
	template <typename Key, typename Val,
		typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Key, Types::ClientID>, std::is_same<Key, std::string>>,
		std::disjunction<std::is_same<Val, Types::PublicKey>, std::is_same<Val, Types::AESKey>>>>>
	void update(const Key& key, const Val& val)
	{
		for (auto& [k, v] : _clients)
		{
			if (std::get<Key>(k) == key)
			{
				std::get<Val>(v) = val;
				return;
			}
		}

		throw std::exception("Client not found");
	}

	template <typename Key,
		typename = std::enable_if_t<std::disjunction_v<std::is_same<Key, Types::ClientID>, std::is_same<Key, std::string>>>>
	bool has_key(const Key& key)
	{
		for (const auto& [k, v] : _clients)
		{
			if (std::get<Key>(k) == key)
			{
				return true;
			}
		}

		return false;
	}

	template <typename Val, typename Key,
		typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Key, Types::ClientID>, std::is_same<Key, std::string>>,
		std::disjunction<std::is_same<Val, Types::PublicKey>, std::is_same<Val, Types::AESKey>>>>>
	bool has_val(const Key& key)
	{
		for (const auto& [k, v] : _clients)
		{
			if (std::get<Key>(k) == key)
			{
				Val empty_val;
				empty_val.fill(0);

				return std::get<Val>(v) != empty_val;
			}
		}

		throw std::exception("Client not found");
	}

	template <typename Key>
	ClientField get_client(const Key& key)
	{
		for (const auto& [k, v] : _clients)
		{
			if (std::get<Key>(k) == key)
			{
				return ClientField(k, v);
			}
		}

		throw std::exception("Client not found");
	}

private:
	std::map<MapKey, MapValue> _clients;
};
