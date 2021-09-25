#include <iostream>
#include "MessageUMenu.h"
#include "ProtocolUtils.h"
#include "ProtocolCommon.h"
#include "StringUtils.h"
#include "Exceptions.h"
#include "Common.h"
#include "GetPublicKeyAction.h"

const std::string GetPublicKeyAction::MENU_NAME = "Request for public key";

void GetPublicKeyAction::execute(MessageUMenu& menu)
{
	char client_name[Common::MAX_CLIENT_NAME_LENGTH];
	std::cout << "Enter client name: ";
	std::cin.ignore();
	std::cin.getline(client_name, Common::MAX_CLIENT_NAME_LENGTH);

	auto requested_client_id = menu.get_client_id(client_name);

	// Create connection with the server
	SocketStream sock(menu.get_server_host());

	// Send request
	Protocol::Utils::send_request_header(sock, menu.get_client()->get_id(), Protocol::RequestCode::PublicKeyRequest, Common::CLIENT_IDENTIFIER_SIZE_BYTES);
	sock.write(StringUtils::to_string(requested_client_id));

	auto header = Protocol::Utils::get_response_header(sock);
	const auto expected_payload_size = Common::CLIENT_IDENTIFIER_SIZE_BYTES + Common::PUBLIC_KEY_SIZE;
	if (header.code != Protocol::ResponseCode::PublicKeySent ||
		expected_payload_size != header.payload_size)
	{
		throw ServerErrorException();
	}

	// Read client ID
	auto id = StringUtils::to_client_id(sock.read(Common::CLIENT_IDENTIFIER_SIZE_BYTES));
	auto public_key = sock.read(Common::PUBLIC_KEY_SIZE);
	if (id != requested_client_id)
	{
		throw ServerErrorException();
	}

	menu.add_public_key(id, public_key);
}
