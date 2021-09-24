#include <sstream>
#include "FileStream.h"
#include "StringUtils.h"
#include "Base64Wrapper.h"
#include "Client.h"

std::string decode_private_key(const std::string& encoded_private_key)
{
	return Base64Wrapper::decode(encoded_private_key);
}

Client* Client::client_from_file(FileStream& info_file)
{
	auto info = info_file.read(info_file.get_file_size());
	if (info.empty())
	{
		throw std::exception("Client info not found");
	}
	
	std::stringstream info_stream(info);

	// Get name from file first line
	std::string name;
	std::getline(info_stream, name);
	if (name.size() > Common::MAX_USER_NAME_LENGTH)
	{
		throw std::exception("Invalid file name");
	}

	// Get client ID from file second line
	std::string identifier;
	std::getline(info_stream, identifier);
	if (identifier.size() != Common::CLIENT_IDENTIFIER_STR_LENGTH)
	{
		throw std::exception("Invalid user indentifier");
	}

	// Get private key from the rest of the file
	std::string encoded_private_key(std::istreambuf_iterator<char>(info_stream), {});
	auto private_key = decode_private_key(encoded_private_key);

	return new Client(name, StringUtils::to_client_id(identifier), private_key);
}

Client* Client::client_from_file(FileStream&& info_file)
{
	return Client::client_from_file(info_file);
}

Client::Client(const std::string& name, const Types::ClientID& identifier, const std::string& private_key) :
	_name(name),
	_identifier(identifier),
	_private_key(RSAPrivateWrapper(private_key)) {}
