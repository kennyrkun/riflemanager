#include "AdminState.hpp"

#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	BACK,
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
	case CALLBACK::BACK:
		app->PopState();
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
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
