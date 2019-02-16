#include "AppEngine.hpp"
#include "RifleListState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>

int main()
{
	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/square.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
	SFUI::Theme::PADDING = 2.f;

	{
		AppEngine app;
		AppSettings settings;
		app.Init("KunLauncher", settings);

		app.window = new sf::RenderWindow(sf::VideoMode(250, 300), "Rifle Manager");
		app.window->setVerticalSyncEnabled(true);

		app.PushState(new RifleListState);

		while (app.Running())
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
	}

	return 0;
}
