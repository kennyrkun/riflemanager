#include "DebugState.hpp"

#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	BACK,
	CHECK_OUT_ALL,
	CHECK_IN_ALL,
};

void DebugState::Init(AppEngine* app)
{
	logger::INFO("DebugState Init.");

	this->app = app;

	menu = buildMainMenu();

	logger::INFO("DebugState Ready.");
}

void DebugState::Cleanup()
{
	logger::INFO("Cleaing up DebugState.");
	logger::INFO("DebugState cleaned up.");
}

void DebugState::Pause()
{
	logger::INFO("DebugState paused.");
}

void DebugState::Resume()
{
	logger::INFO("DebugState resumed.");
}

void DebugState::HandleEvents()
{
	sf::Event event;

	while (app->window->pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
			app->Quit();
		else if (event.type == sf::Event::EventType::Resized)
		{
			sf::Vector2u newSize(event.size.width, event.size.height);

			sf::FloatRect visibleArea(0.0f, 0.0f, event.size.width, event.size.height);
			app->window->setView(sf::View(visibleArea));

		}
	}

	int id = menu->onEvent(event);

	switch (id)
	{
	case CALLBACK::CHECK_IN_ALL:
		app->rm.returnAllRifles();
		break;
	case CALLBACK::CHECK_OUT_ALL:
		app->rm.checkoutAllRifles();
		break;
	case CALLBACK::BACK:
		app->PopState();
		break;
	default:
		break;
	}
}

void DebugState::Update()
{
}

void DebugState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* DebugState::buildMainMenu()
{
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("DebugState");

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Check In All", CALLBACK::CHECK_IN_ALL);
	newMenu->addButton("Check Out All", CALLBACK::CHECK_OUT_ALL);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
