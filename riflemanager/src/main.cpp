#include "AppEngine.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

int main(int argc, char* argv[])
{
	#ifdef IS_DEBUG_ENABLED
	logger::WARNING("debug is enabled?");
	#endif

	#ifdef NDEBUG
	logger::WARNING("Running in Debug mode. (NDEBUG is defined)");
	#else // release probably
	logger::WARNING("Running in Release mode... probably. (NDEBUG is not defined.)");
	#endif

	AppEngine app;
	AppSettings settings;
	settings.commandline = { argc, argv };

	app.Initialise("Rifle Manager " + std::to_string(settings.version), settings);

	// FIXME: this should actually be set in app.Initialise
	app.window->setVerticalSyncEnabled(true);

#ifdef NDEBUG
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
