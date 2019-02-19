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

		if (id == 100)
		{
			app->ChangeState(new MainMenuState);
		}

		/*
		if (id != -1)
			if (id < rm.riflesOut.size())
			{
//				std::cout << rm.riflesOut[id].second + "'s Rifle is out! (" << rm.riflesOut[id].first << ")" << std::endl;

				try
				{
					rm.riflesOut.erase(std::remove(rm.riflesOut.begin(), rm.riflesOut.end(), rm.riflesOut[id]), rm.riflesOut.end());
				}
				catch (std::exception e)
				{
					std::cerr << "failed to remove rifle from list" << std::endl;
				}

				menu = buildRifleMenu();

				if (!menu)
					std::cerr << "new menu is null" << std::endl;
			}
		*/
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
		menu->addButton(std::to_string(app->rm.rifles[i]), i);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	SFUI::InputBox* name = new SFUI::InputBox;
	SFUI::InputBox* rifleID = new SFUI::InputBox(47);

	menu->add(name, -2);
	menu->add(rifleID, -3);
	menu->addButton("Sign Out");
	menu->addButton("Back", 100);

	return menu;
}
