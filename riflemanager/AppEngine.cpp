#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFUI/Theme.hpp>
#include <iostream>
#include <ctime>

// TODO: load program icon

void AppEngine::Init(std::string title_, AppSettings settings_)
{
	std::cout << "AppEngine Init" << std::endl;

	settings = settings_;

	window = new sf::RenderWindow;
	window->setVerticalSyncEnabled(settings.window.verticalSync);

	/*
	SFUI::Theme::loadFont(GBL::DIR::fonts + "Arial.ttf");
	SFUI::Theme::loadTexture(GBL::DIR::textures + "interface_square.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#fff");
	SFUI::Theme::windowBgColor = GBL::theme.palatte.TERTIARY;
	SFUI::Theme::PADDING = 2.f;
	*/

	running = true;
}

void AppEngine::Cleanup()
{
	std::cout << "Cleaning up AppEngine." << std::endl;
	
	for (size_t i = 0; i < states.size(); i++)
		PopState();

	window->close();
	delete window;

	std::cout << "AppEngine cleaned up." << std::endl;
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

void AppEngine::UpdateViewSize(const sf::Vector2f& size)
{
	std::cout << "new width: " << size.x << std::endl;
	std::cout << "new height: " << size.y << std::endl;

	sf::Vector2u newSize(size.x, size.y);

	//TODO: define max size somewhere
	if (newSize.x >= 525 && newSize.y >= 325)
	{
		sf::FloatRect visibleArea(0, 0, size.x, size.y);
		window->setView(sf::View(visibleArea));
	}
	else
	{
		if (size.x <= settings.window.width)
			newSize.x = settings.window.width;

		if (size.x <= settings.window.height)
			newSize.y = settings.window.height;

		window->setSize(newSize);
	}
}

// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c/10467633#10467633
const std::string AppEngine::currentDateTime() 
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];
	localtime_s(&timeinfo, &now);
	strftime(buf, sizeof(buf), "%F.%H-%M-%S", &timeinfo);

	return buf;
}
