#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Logger.hpp"
#include "SettingsParser.hpp"

#include <SFUI/Theme.hpp>

#include <ctime>

void AppEngine::Initialise(const std::string& title, const AppSettings& settings_)
{
	logger::DEBUG("[APP ENGINE]: Initialising...");

	settings = settings_;

	window = new sf::RenderWindow(sf::VideoMode(settings.window.width, settings.window.height), title);
	window->setVerticalSyncEnabled(settings.window.verticalSync);

	SettingsParser parser;

	if (parser.loadFromFile("./resources/interface/SFUI.conf"))
	{
		logger::INFO("It looks like a custom SFUI configuration has been specified. RifleManager will attempt to load it, but will employ no safeties while doing so.");
		logger::WARNING("Use custom configurations at your own risk.");

		std::string font, texture, textCharacterSize, clickTextColor, clickTextColorHover, clickTextColorFocus, inputTextColor, inputTextColorHover, inputTextColorFocus, windowBgColor, PADDING, debug;
		parser.get("font", font);
		parser.get("texture", texture);
		parser.get("textCharacterSize", textCharacterSize);
		parser.get("clickTextColor", clickTextColor);
		parser.get("clickTextColorHover", clickTextColorHover);
		parser.get("clickTextColorFocus", clickTextColorFocus);
		parser.get("inputTextColor", inputTextColor);
		parser.get("inputTextColorHover", inputTextColorHover);
		parser.get("inputTextColorFocus", inputTextColorFocus);
		parser.get("windowBgColor", windowBgColor);
		parser.get("PADDING", PADDING);
		parser.get("debug", debug);

		SFUI::Theme::loadFont(font);
		SFUI::Theme::loadTexture(texture, (debug == "TRUE" ? true : false) || settings.SFUIDebug);
		SFUI::Theme::textCharacterSize = std::stoi(textCharacterSize);
		SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb(clickTextColor);
		SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb(clickTextColorHover);
		SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb(clickTextColorFocus);
		SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb(inputTextColor);
		SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb(inputTextColorHover);
		SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb(inputTextColorFocus);
		SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb(windowBgColor);
		SFUI::Theme::PADDING = std::stof(PADDING);
	}
	else
	{
		SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
		SFUI::Theme::loadTexture("resources/interface/square.png", settings.SFUIDebug);
		SFUI::Theme::textCharacterSize = 11;
		SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
		SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
		SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
		SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
		SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
		SFUI::Theme::PADDING = 2.f;
	}

	rm.loadRifleData();

	running = true;

	logger::DEBUG("[APP ENGINE]: Ready.");
}

void AppEngine::Cleanup()
{
	logger::DEBUG("[APP ENGINE]: Cleaning up...");
	
	for (size_t i = 0; i < states.size(); i++)
		PopState();

	delete window;

	logger::DEBUG("[APP ENGINE]: cleaned up.");
}

void AppEngine::ChangeState(AppState* state)
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::ChangeState, state));
}

void AppEngine::PushState(AppState* state)
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::PushState, state));
}

void AppEngine::PopState()
{
	queuedEvents.push_back(std::pair<EventType, AppState*>(EventType::PopState, nullptr));
}

void AppEngine::HandleEvents()
{
	if (running && !states.empty())
		states.back()->HandleEvents();

	for (size_t i = 0; i < queuedEvents.size(); i++)
	{
		if (queuedEvents[i].first == EventType::ChangeState)
		{
			if (!states.empty())
			{
				states.back()->Cleanup();

				delete states.back();
				states.pop_back();
			}

			states.push_back(queuedEvents[i].second);
			states.back()->Init(this);
		}
		else if (queuedEvents[i].first == EventType::PushState)
		{
			if (!states.empty())
				states.back()->Pause();

			states.push_back(queuedEvents[i].second);
			states.back()->Init(this);
		}
		else if (queuedEvents[i].first == EventType::PopState)
		{
			if (!states.empty())
			{
				states.back()->Cleanup();

				delete states.back();
				states.pop_back();
			}

			if (!states.empty())
				states.back()->Resume();
		}
		else if (queuedEvents[i].first == EventType::Quit)
		{
			// TODO: implement bobwars appengine quit
		}

		// this might break things, but I'm not sure.
		// I am confident it will work.
		queuedEvents.pop_back();
	}
}

void AppEngine::Update()
{
	if (running)
		states.back()->Update();
}

void AppEngine::Draw()
{
	if (running)
		states.back()->Draw();
}

// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c/10467633#10467633
const std::string AppEngine::getTimestamp() 
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];

#ifdef _WIN32
	localtime_s(&timeinfo, &now);
#else
	localtime_r(&now, &timeinfo);
#endif

	strftime(buf, sizeof(buf), "%F.%H-%M-%S", &timeinfo);

	return buf;
}
