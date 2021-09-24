#pragma once

#include <string>
#include "IAction.h"
#include "Common.h"

class ClientsListAction : public IAction
{
public:
	static const std::string MENU_NAME;

public:
	ClientsListAction() = default;
	virtual ~ClientsListAction() = default;

	virtual void execute(MessageUMenu& menu) override;

private:
	Types::ClientID _get_response();
};

