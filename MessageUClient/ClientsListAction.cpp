#include "ProtocolUtils.h"
#include "MessageUMenu.h"
#include "ClientsListAction.h"

const std::string ClientsListAction::MENU_NAME = "Request for clients list";

void ClientsListAction::execute(MessageUMenu& menu)
{
	SocketStream sock(menu.get_server_host());
	Protocol::Utils::send_request_header(sock, menu.get_client()->get_id(), Protocol::RequestCode::ClientsList, 0);
}

void ClientsListAction::_handle_response(MessageUMenu& menu, SocketStream& sock)
{
	// Read response header
	auto header = Protocol::Utils::get_response_header(sock);
	if (Protocol::ResponseCode::ClientsListSent != header.code)
	{
		throw std::exception("Failed retreive clients list");
	}

	const int clients_count = header.payload_size / (Common::CLIENT_IDENTIFIER_SIZE_BYTES + Common::MAX_CLIENT_NAME_LENGTH);
	for (int i = 1; i <= clients_count; ++i)
	{
		auto id = StringUtils::to_client_id(sock.read(Common::CLIENT_IDENTIFIER_SIZE_BYTES));
		auto name = sock.read(Common::MAX_CLIENT_NAME_LENGTH);
		std::cout << i << ") " << name << std::endl;

		menu.add_client_id(name, id);
	}
}
