#pragma once

#include <string>
#include "IAction.h"

class ExitAction : public IAction
{
public:
	static const std::string MENU_NAME;

public:
	virtual void execute(MessageUMenu&) override;
};
