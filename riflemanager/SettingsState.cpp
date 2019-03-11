#include "SettingsState.hpp"

#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	BACK,
};

void SettingsState::Init(AppEngine* app)
{
	logger::INFO("SettingsState Init.");

	this->app = app;

	menu = buildMainMenu();

	logger::INFO("SettingsState Ready.");
}

void SettingsState::Cleanup()
{
	logger::INFO("Cleaing up SettingsState.");
	logger::INFO("SettingsState cleaned up.");
}

void SettingsState::Pause()
{
	logger::INFO("SettingsState paused.");
}

void SettingsState::Resume()
{
	logger::INFO("SettingsState resumed.");
}

void SettingsState::HandleEvents()
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
	case CALLBACK::BACK:
		app->PopState();
		break;
	default:
		break;
	}
}

void SettingsState::Update()
{
}

void SettingsState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* SettingsState::buildMainMenu()
{
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
