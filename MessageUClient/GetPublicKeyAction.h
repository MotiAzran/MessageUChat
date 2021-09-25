#pragma once

#include <string>
#include "IAction.h"

class GetPublicKeyAction : public IAction
{
public:
	static const std::string MENU_NAME;

public:
	GetPublicKeyAction() = default;
	virtual ~GetPublicKeyAction() = default;

	virtual void execute(MessageUMenu& menu) override;
};
