#pragma once

#include <string>
#include "IAction.h"
#include "RSAWrapper.h"

using Types::Host;

class RegisterAction : public IAction
{
public:
	static const std::string MENU_NAME;

public:
	RegisterAction() = default;
	virtual ~RegisterAction() = default;

	virtual void execute(MessageUMenu& menu) override;

private:
	bool _can_register();
	void _send_request(SocketStream& sock, const std::string& username, const RSAPrivateWrapper& rsapriv);
	Types::ClientID _get_response(SocketStream& sock);
	void _write_client_info(const std::string& username, const Types::ClientID& id, const RSAPrivateWrapper& rsapriv);
};
