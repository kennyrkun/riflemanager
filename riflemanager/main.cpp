#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

int main()
{
	AppEngine app;
	AppSettings settings;
	app.Initialise("Rifle Manager 1.1", settings);

	// FIXME: this should actually be set in app.Initialise
	app.window->setVerticalSyncEnabled(true);

	app.PushState(new MainMenuState);

	while (app.isRunning())
	{
		app.HandleEvents();
		app.Update();
		app.Draw();
	}

	app.Cleanup();

	logger::INFO("Shutdown looks good. Have a beautiful time ;)");

	return 0;
}
