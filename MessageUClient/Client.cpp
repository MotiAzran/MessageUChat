#include "FileStream.h"
#include "BufferUtils.h"
#include "StringUtils.h"
#include "Client.h"

Client Client::client_from_file()
{
	enum class SplitedIndex : int
	{
		Name = 1,
		Identifier,
		PrivKey,

		// Must be last
		Count
	};

	FileStream client_info(Common::CLIENT_INFO_PATH);
	auto info = BufferUtils::buffer_to_string(client_info.read(client_info.get_file_size()));
	if (info.empty())
	{
		throw std::exception("Client info not found");
	}

	auto splited = StringUtils::split(info, '\n');
	if (static_cast<int>(SplitedIndex::Count) != splited.size())
	{
		throw std::exception("Invalid client info file format");
	}

	auto name = splited[static_cast<int>(SplitedIndex::Name)];
	if (name.size() > Common::MAX_USER_NAME_LENGTH)
	{
		throw std::exception("Invalid file name");
	}

	auto identifier = splited[static_cast<int>(SplitedIndex::Identifier)];
	if (identifier.size() != Common::USER_IDENTIFIER_LENGTH)
	{
		throw std::exception("Invalid user indentifier");
	}

	auto private_key = splited[static_cast<int>(SplitedIndex::PrivKey)];

	return Client(name, identifier, private_key);
}

Client::Client(const std::string& name, const std::string& identifier, const std::string& private_key) :
	_name(name),
	_identifier(identifier),
	_private_key(private_key) {}
