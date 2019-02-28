#include "AppEngine.hpp"
#include "RifleListState.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"
#include "RifleFileSystem.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

enum CALLBACKS
{
	BACK,
	SHOW_ALL_RIFLES
};

void RifleListState::Init(AppEngine* app_)
{
	logger::INFO("RifleListState Init");
	app = app_;

	menu = buildRifleMenu();

	logger::INFO("RifleListState ready");
}

void RifleListState::Cleanup()
{
	logger::INFO("Cleaning up RifleListState");

	logger::INFO("RifleListState Cleanup");
}

void RifleListState::Pause()
{
	logger::INFO("RifleListState paused");
}

void RifleListState::Resume()
{
	logger::INFO("RifleListState resumed");
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

		switch (id)
		{
			case CALLBACKS::BACK:
			{
				app->ChangeState(new MainMenuState);
				break;
			}
			case CALLBACKS::SHOW_ALL_RIFLES:
			{
				showAllRifles = !showAllRifles;
				menu = buildRifleMenu();
				break;
			}
			default:
			{
				if (id != -1)
				{
					logger::INFO("want to return rifle " + std::to_string(id));

					app->rm.returnRifle(id);
					menu = buildRifleMenu();
				}
			}
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

	menu->addLabel("Rifles List:");

	std::vector<rifle::serial> rifles = rfs::getRifleList();

	for (size_t i = 0; i < rifles.size(); i++)
	{
		if (app->rm.isRifleOut(rifles[i]))
			menu->addButton(std::to_string(app->rm.rifles[i]) + "/" + rifle::loadInfo(app->rm.rifles[i]).user, app->rm.rifles[i]);
		else if (showAllRifles)
			menu->addLabel(std::to_string(rifles[i]));
	}

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	SFUI::FormLayout* form = menu->addFormLayout();
	showAllRiflesBox = new SFUI::CheckBox(showAllRifles);
	form->addRow("Show All Rifles", showAllRiflesBox, CALLBACKS::SHOW_ALL_RIFLES);

	menu->addButton("Back", CALLBACKS::BACK);

	return menu;
}
