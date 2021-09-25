#include "Exceptions.h"
#include "ExitAction.h"

const std::string ExitAction::MENU_NAME = "Exit client";

void ExitAction::execute(MessageUMenu&)
{
	throw ExitException();
}
