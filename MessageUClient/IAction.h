#pragma once

class MessageUMenu;

class IAction
{
public:
	virtual void execute(MessageUMenu& menu) = 0;
};
