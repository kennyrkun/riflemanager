#include "AdminState.hpp"
#include "DebugState.hpp"
#include "SettingsState.hpp"

#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	BACK,
	DEBUG,
	SETTINGS,
	SIGN_OUT_ALL,
	SIGN_IN_ALL
};

void AdminState::Init(AppEngine* app)
{
	logger::INFO("AdminState Init.");

	this->app = app;

	menu = buildMainMenu();

	logger::INFO("AdminState Ready.");
}

void AdminState::Cleanup()
{
	logger::INFO("Cleaing up AdminState.");
	logger::INFO("AdminState cleaned up.");
}

void AdminState::Pause()
{
	logger::INFO("AdminState paused.");
}

void AdminState::Resume()
{
	logger::INFO("AdminState resumed.");
}

void AdminState::HandleEvents()
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
	case CALLBACK::SETTINGS:
		app->PushState(new SettingsState);
		break;
	case CALLBACK::DEBUG:
		app->PushState(new DebugState);
		break;
	case CALLBACK::BACK:
		app->PopState();
		break;
	case CALLBACK::SIGN_OUT_ALL:
		app->rm.checkoutAllRifles();
		break;
	case CALLBACK::SIGN_IN_ALL:
		app->rm.checkoutAllRifles();
		break;
	default:
		break;
	}
}

void AdminState::Update()
{
}

void AdminState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* AdminState::buildMainMenu()
{
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(8, 10));

	newMenu->addLabel("AdminState");

	newMenu->addButton("Settings", CALLBACK::SETTINGS);
	newMenu->addButton("Debug", CALLBACK::DEBUG);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Checkout all Rifles", CALLBACK::SIGN_OUT_ALL);
	newMenu->addButton("Return all Rifles", CALLBACK::SIGN_IN_ALL);

#ifndef PLATFORM_TOUCH
	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK::BACK);
#endif

	return newMenu;
}
