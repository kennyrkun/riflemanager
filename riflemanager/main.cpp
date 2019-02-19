#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>

int main()
{
	AppEngine app;
	AppSettings settings;
	app.Init("KunLauncher", settings);

	app.window = new sf::RenderWindow(sf::VideoMode(250, 300), "Rifle Manager");
	app.window->setVerticalSyncEnabled(true);

	app.PushState(new MainMenuState);

	while (app.isRunning())
	{
		try
		{
			app.HandleEvents();
			app.Update();
			app.Draw();
		}
		catch (std::exception& e)
		{
			std::cerr << "[EXCEPTION/MAIN]: " << e.what() << std::endl;
		}
	}

	app.Cleanup();

	return 0;
}
