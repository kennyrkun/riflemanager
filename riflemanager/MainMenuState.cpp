#include "AppEngine.hpp"
#include "MainMenuState.hpp"
#include "RifleListState.hpp"
#include "RifleCheckoutState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

enum CALLBACK
{
	RIFLE_LIST,
	CHECKOUT_RIFLE,
	SETTINGS,
	ADMIN,
	ABOUT,
	EXIT
};

void MainMenuState::Init(AppEngine* app_)
{
	logger::INFO("MainMenuState Init");
	app = app_;

	menu = buildMainMenu();

	logger::INFO("MainMenuState ready");
}

void MainMenuState::Cleanup()
{
	logger::INFO("Cleaning up MainMenuState");

	logger::INFO("MainMenuState Cleanup");
}

void MainMenuState::Pause()
{
	logger::INFO("MainMenuState paused");
}

void MainMenuState::Resume()
{
	logger::INFO("MainMenuState resumed");
}

void MainMenuState::HandleEvents()
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

		switch (id)
		{
		case CALLBACK::CHECKOUT_RIFLE:
			app->ChangeState(new RifleCheckoutState);
			break;
		case CALLBACK::RIFLE_LIST:
			app->ChangeState(new RifleListState);
			break;
		case CALLBACK::SETTINGS:
		case CALLBACK::ADMIN:
		case CALLBACK::ABOUT:
			break;
		case CALLBACK::EXIT:
			app->Quit();
			break;
		}
	}
}

void MainMenuState::Update()
{
}

void MainMenuState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);
	
	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* MainMenuState::buildMainMenu()
{
	SFUI::Menu* menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(10, 10));

	menu->addButton("Rifle List", CALLBACK::RIFLE_LIST);
	menu->addButton("Checkout Rifle", CALLBACK::CHECKOUT_RIFLE);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

#ifdef _DEBUG
	menu->addButton("Settings", CALLBACK::SETTINGS);
	menu->addButton("Admin", CALLBACK::ADMIN);
	menu->addButton("About", CALLBACK::ABOUT);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();
#endif

	menu->addButton("Exit", CALLBACK::EXIT);

	return menu;
}
