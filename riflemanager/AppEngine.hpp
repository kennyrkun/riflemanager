#ifndef PROGRAM_ENGINE_HPP
#define PROGRAM_ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <thread>

// APP ENGINE REVISION: 1

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
		int width = 525;
		int	height = 375;
	} window;
};

class AppEngine
{
public:
	void Init(std::string title, AppSettings settings_);
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

	bool Running() { return running; }
	// THE ACTIVE STATE IS NOT SWITCHED UNTIL HandleEvents() HAS RETURNED
	void Quit() { running = false; }

	void UpdateViewSize(const sf::Vector2f& size);

	const std::string currentDateTime();

	sf::RenderWindow* window;
	AppSettings settings;

private:
	// the stack of states
	std::vector<AppState*> states;

	enum class EventType
	{
		PushState,
		ChangeState,
		PopState,
		Quit
	};

	std::vector<std::pair<EventType, AppState*>> queuedEvents;

	bool running;
};

#endif // !PROGRAM_ENGINE_HPP
