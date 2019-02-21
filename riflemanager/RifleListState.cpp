#include "AppEngine.hpp"
#include "RifleListState.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void RifleListState::Init(AppEngine* app_)
{
	std::cout << "RifleListState Init" << std::endl;
	app = app_;

	menu = buildRifleMenu();

	std::cout << "RifleListState ready" << std::endl;
}

void RifleListState::Cleanup()
{
	std::cout << "Cleaning up RifleListState" << std::endl;

	std::cout << "RifleListState Cleanup" << std::endl;
}

void RifleListState::Pause()
{
	std::cout << "RifleListState paused" << std::endl;
}

void RifleListState::Resume()
{
	std::cout << "RifleListState resumed" << std::endl;
}

void RifleListState::HandleEvents()
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

		int id = menu->onEvent(event);

		if (id == -2)
			app->ChangeState(new MainMenuState);

		if (id != -1)
		{
			std::cout << "want to return rifle " << id << std::endl;

			app->rm.returnRifle(id);
			menu = buildRifleMenu();
		}
	}
}

void RifleListState::Update()
{
}

void RifleListState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);
	app->window->draw(*menu);
	app->window->display();
}

SFUI::Menu* RifleListState::buildRifleMenu()
{
	SFUI::Menu* menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(10, 10));

	menu->addLabel("Rifles Out:");

	for (size_t i = 0; i < app->rm.rifles.size(); i++)
		if (app->rm.isRifleOut(app->rm.rifles[i]))
			menu->addButton(std::to_string(app->rm.rifles[i]), app->rm.rifles[i]);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	menu->addButton("Back", -2);

	return menu;
}
