#include "FileStream.h"
#include "BufferUtils.h"
#include "StringUtils.h"
#include "MessageUClient.h"

MessageUClient::MessageUClient() :
	_server_host(_get_server_host())
{}

SocketStream::Host MessageUClient::_get_server_host()
{
	enum class SplitedIndex : int
	{
		IP = 1,
		Port,

		// Must be last
		Count
	};

	FileStream server_info(Common::SERVER_INFO_PATH);
	auto info = BufferUtils::buffer_to_string(server_info.read(server_info.get_file_size()));
	if (info.empty())
	{
		throw std::exception("Server info not found");
	}

	auto splited_str = StringUtils::split(info, ':');
	if (static_cast<int>(SplitedIndex::Count) != splited_str.size())
	{
		throw std::exception("Invalid server info file format");
	}

	auto ip_str = splited_str[static_cast<int>(SplitedIndex::IP)];

	auto port_str = splited_str[static_cast<int>(SplitedIndex::Port)];
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
