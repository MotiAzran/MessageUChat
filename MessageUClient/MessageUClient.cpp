#include "FileStream.h"
#include "BufferUtils.h"
#include "StringUtils.h"
#include "MessageUClient.h"

MessageUClient::MessageUClient() :
	_server_host(_get_server_host())
{
	_initialize_client_info();
}

void MessageUClient::_initialize_client_info()
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

	_name = splited[static_cast<int>(SplitedIndex::Name)];
	if (_name.size() > Common::MAX_USER_NAME_LENGTH)
	{
		throw std::exception("Invalid file name");
	}

	_identifier = splited[static_cast<int>(SplitedIndex::Identifier)];
	if (_identifier.size() != Common::USER_IDENTIFIER_LENGTH)
	{
		throw std::exception("Invalid user indentifier");
	}

	_private_key = splited[static_cast<int>(SplitedIndex::PrivKey)];
}

SocketStream::Host MessageUClient::_get_server_host()
{
	const auto EXEPECTED_INFO_PARTS_COUNT = 2;

	FileStream server_info(Common::SERVER_INFO_PATH);
	auto info = BufferUtils::buffer_to_string(server_info.read(server_info.get_file_size()));
	if (info.empty())
	{
		throw std::exception("Server info not found");
	}

	auto splited_str = StringUtils::split(info, ':');
	if (EXEPECTED_INFO_PARTS_COUNT != splited_str.size())
	{
		throw std::exception("Invalid server info file format");
	}

	auto ip_str = splited_str[0];

	auto port_str = splited_str[1];
	if (StringUtils::is_number(port_str))
	{
		throw std::exception("Invalid port");
	}

	auto port = std::stoi(port_str);
	if (0 > port || Common::MAX_PORT < port)
	{
		throw std::exception("Invalid port value");
	}

	return SocketStream::Host(IPAddress(ip_str), static_cast<SocketStream::Port>(port));
}
