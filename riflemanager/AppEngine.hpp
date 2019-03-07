#ifndef PROGRAM_ENGINE_HPP
#define PROGRAM_ENGINE_HPP

#include "RifleManager.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>

// APP ENGINE REVISION: 7

class AppState;

// TODO: don't set these if they've been overriden via command line arugements
struct AppSettings
{
	bool SFUIDebug = false;
	bool useAnimations = false;
	int animationScale = 1;

	struct Window
	{
		bool verticalSync = true;
		int width = 250;
		int	height = 300;
	} window;

	struct CommandLineArguments
	{
		int argc;
		char** argv;
	} commandline;
};

class AppEngine
{
public:
	void Initialise(const std::string& title, AppSettings settings_);
	void Cleanup();

	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void ChangeState(AppState* state);
	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void PushState(AppState* state);
	// THE ACTIVE STATE IS NOT SWITCHED UNTIL THE CALLER FUNCTION HAS RETURNED
	void PopState();

	AppState* GetCurrentState() { return states.back(); };

	void HandleEvents();
	void Update();
	void Draw();

	bool isRunning() { return running; }

	// THE ACTIVE STATE IS NOT SWITCHED UNTIL HandleEvents() HAS RETURNED
	void Quit() { running = false; }

	const std::string getTimestamp();

	sf::RenderWindow* window;
	AppSettings settings;

	RifleManager rm;

private:
	enum class EventType
	{
		PushState,
		ChangeState,
		PopState,
		Quit
	};

	std::vector<std::pair<EventType, AppState*>> queuedEvents;

	std::vector<AppState*> states;

	bool running;
};

#endif // !PROGRAM_ENGINE_HPP
