#include "ProtocolUtils.h"
#include "ClientsListAction.h"

const std::string ClientsListAction::MENU_NAME = "Request for clients list";

void ClientsListAction::execute(MessageUMenu& menu)
{
	//Protocol::Utils::send_request_header(sock, client.)
}

Types::ClientID ClientsListAction::_get_response()
{
	return Types::ClientID();
}
