#pragma once

#include <type_traits>
#include "IAction.h"

class IActionFactory
{
public:
	virtual IAction* create() = 0;
	virtual std::string get_menu_name() = 0;
};

template <typename Action, typename = std::enable_if_t<std::is_base_of_v<IAction, Action>>>
class ActionFactory : public IActionFactory
{
public:
	virtual IAction* create() override { return new Action(); }
	virtual std::string get_menu_name() override { return Action::MENU_NAME; }
};
