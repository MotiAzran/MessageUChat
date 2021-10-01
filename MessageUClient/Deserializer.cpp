#define NOMINMAX

#include "Deserializer.h"

Deserializer::Deserializer(std::string&& data) : 
	_data(std::move(data)) {}

std::string Deserializer::read(const uint32_t size)
{
	if (_data.empty())
	{
		throw std::exception("No more data to read");
	}

	if (0 == size)
	{
		return std::string();
	}

	uint32_t size_to_read = std::min(size, _data.size());
	std::string read_data(_data.begin(), _data.begin() + size_to_read);

	_data.erase(0, size_to_read);

	return read_data;
}

Types::ClientID Deserializer::read_client_id()
{
	if (_data.size() < Common::CLIENT_ID_SIZE_BYTES)
	{
		throw std::out_of_range("Not enuogh data");
	}

	return StringUtils::to_client_id(read(Common::CLIENT_ID_SIZE_BYTES));
}

Types::PublicKey Deserializer::read_public_key()
{
	if (_data.size() < Common::PUBLIC_KEY_SIZE)
	{
		throw std::out_of_range("Not enough data");
	}

	return StringUtils::to_public_key(read(Common::PUBLIC_KEY_SIZE));
}
