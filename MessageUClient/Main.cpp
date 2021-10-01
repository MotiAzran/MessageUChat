#include <iostream>
#include "AutoWSA.h"
#include "MessageUMenu.h"
#include "Exceptions.h"

int main()
{
	try
	{
		AutoWSA wsa;
		MessageUMenu menu;

		std::cout << "MessageU client at your service." << std::endl;
		while (true)
		{
			menu.print();

			uint32_t client_choice = 0;
			std::cout << "? ";
			std::cin >> client_choice;
			std::cin.ignore();

			try
			{
				menu.handle_choice(client_choice);
			}
			catch (const ExitException&)
			{
				std::cout << "Bye bye" << std::endl;
				return 0;
			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown error" << std::endl;
	}

	return 0;
}