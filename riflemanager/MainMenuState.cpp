#include "AppEngine.hpp"
#include "MainMenuState.hpp"
#include "RifleListState.hpp"
#include "RifleCheckoutState.hpp"
#include "AboutState.hpp"
#include "AdminState.hpp"

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

enum CALLBACK
{
	RIFLE_LIST,
	CHECKOUT_RIFLE,
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

	delete menu;

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
			app->PushState(new RifleCheckoutState);
			return;
			break;
		case CALLBACK::RIFLE_LIST:
			app->PushState(new RifleListState);
			return;
			break;
		case CALLBACK::ADMIN:
			app->PushState(new AdminState);
			break;
		case CALLBACK::ABOUT:
			app->PushState(new AboutState);
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
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(8, 10));

	SFUI::HorizontalBoxLayout* panelContainer= newMenu->addHorizontalBoxLayout();

	SFUI::VerticalBoxLayout* leftPanel = panelContainer->addVerticalBoxLayout();
	leftPanel->addButton("Rifle List", CALLBACK::RIFLE_LIST);
	leftPanel->addButton("Checkout Rifle", CALLBACK::CHECKOUT_RIFLE);
	leftPanel->addButton("About", CALLBACK::ABOUT);

#ifdef NDEBUG
	SFUI::VerticalBoxLayout* rightPanel = panelContainer->addVerticalBoxLayout();
	rightPanel->addButton("Admin", CALLBACK::ADMIN);
#endif

#ifndef PLATFORM_TOUCH
	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Exit", CALLBACK::EXIT);
#endif

	return newMenu;
}
