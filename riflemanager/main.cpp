#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

int main(int argc, char* argv[])
{
	AppEngine app;
	AppSettings settings;
	settings.commandline = { argc, argv };

	app.Initialise("Rifle Manager 1.2", settings);

	// FIXME: this should actually be set in app.Initialise
	app.window->setVerticalSyncEnabled(true);

#ifdef _DEBUG
	// HACK: keeps program from using 30% cpu while idle
	app.window->setFramerateLimit(60);
#endif

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
