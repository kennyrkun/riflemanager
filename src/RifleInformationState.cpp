#include "RifleInformationState.hpp"

#include "Logger.hpp"
#include "SettingsParser.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK_MAIN
{
	BACK_M
};

enum CALLBACK_EDITOR
{
	RETURN_RIFLE,
	REMOVE_RIFLE,
	CHANGE_RIFLE_TYPE,

	BACK_E
};

void RifleInformationState::Init(AppEngine* app)
{
	logger::INFO("RifleInformationState Init.");

	this->app = app;

	menu = buildRiflePicker();

	logger::INFO("RifleInformationState Ready.");
}

void RifleInformationState::Cleanup()
{
	logger::INFO("RifleInformationState up DebugState.");

	delete menu;

	logger::INFO("RifleInformationState cleaned up.");
}

void RifleInformationState::Pause()
{
	logger::INFO("RifleInformationState paused.");
}

void RifleInformationState::Resume()
{
	logger::INFO("RifleInformationState resumed.");
}

void RifleInformationState::HandleEvents()
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

		if (menuState == MenuState::PickingRifle)
		{
			switch (id)
			{
			case CALLBACK_MAIN::BACK_M:
				app->PopState();
				break;
			default:
			{
				if (id != -1)
				{
					logger::INFO("want to edit rifle " + std::to_string(id));

					delete menu;
					menu = buildRifleEditor(id);
				}
			}
			}
		}
		else if (menuState == MenuState::EditingRifle)
		{
			switch (id)
			{
			case CALLBACK_EDITOR::RETURN_RIFLE:
			{
				app->rm.returnRifle(rifle);

				delete menu;
				menu = buildRifleEditor(rifle);

				break;
			}
			case CALLBACK_EDITOR::REMOVE_RIFLE:
			{
				rfs::removeRifle(rifle);

				delete menu;
				menu = buildRiflePicker();

				break;
			}
			case CALLBACK_EDITOR::CHANGE_RIFLE_TYPE:
			{
				logger::INFO("rifletype");

				SettingsParser parser("./resources/rifleinventory/" + std::to_string(rifle) + "/info.dat");

				parser.set("type", rifleType->getSelectedValue());

				break;
			}
			case CALLBACK_EDITOR::BACK_E:
				delete menu;
				menu = buildRiflePicker();
				break;
			default:
				break;
			}
		}
	}
}

void RifleInformationState::Update()
{
}

void RifleInformationState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* RifleInformationState::buildRiflePicker()
{
	menuState = MenuState::PickingRifle;
	rifle = -1;

	sf::Clock creationTimer;

	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("Rifle List:");

	std::vector<rifle::serial> rifles = rfs::getRifleList();

	for (size_t i = 0; i < rifles.size(); i++)
	{
		if (app->rm.isRifleOut(rifles[i]))
			newMenu->addButton(std::to_string(app->rm.rifles[i]) + "/" + rifle::loadInfo(app->rm.rifles[i]).user, app->rm.rifles[i]);
		else
			newMenu->addButton(std::to_string(rifles[i]), app->rm.rifles[i]);
	}

	newMenu->addHorizontalBoxLayout();
	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK_MAIN::BACK_M);

	sf::Time timeSpentCreatingMenu = creationTimer.getElapsedTime();

	logger::INFO("Menu Creation took " + std::to_string(timeSpentCreatingMenu.asSeconds()) + "s");

	if (timeSpentCreatingMenu.asSeconds() > 1.0f)
		logger::WARNING("Menu Creation took longer than 1000ms!");

	return newMenu;
}

SFUI::Menu* RifleInformationState::buildRifleEditor(rifle::serial rifle)
{
	menuState = MenuState::EditingRifle;
	this->rifle = rifle;

	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("Editing Rifle " + std::to_string(rifle));

	newMenu->addHorizontalBoxLayout();

	newMenu->addLabel("Status: " + app->rm.getRifleInfo(rifle).status);

	if (app->rm.isRifleOut(rifle))
		newMenu->addLabel("User: " + app->rm.getRifleInfo(rifle).user);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Return Rifle", CALLBACK_EDITOR::RETURN_RIFLE);
	newMenu->addButton("Remove Rifle", CALLBACK_EDITOR::REMOVE_RIFLE);

	newMenu->addHorizontalBoxLayout();

	SFUI::FormLayout* form = newMenu->addFormLayout();

	std::string type = app->rm.getRifleInfo(rifle).type;
	std::string type2 = type == "drill" ? "honorguard" : "drill";

	rifleType = new SFUI::OptionsBox<std::string>;
	rifleType->addItem(type2 == "drill" ? "Drill" : "Honor Guard", type2);
	rifleType->addItem(type == "drill" ? " Drill" : "Honor Guard", type);
	form->addRow("Rifle Type:", rifleType, CALLBACK_EDITOR::CHANGE_RIFLE_TYPE);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK_EDITOR::BACK_E);

	return newMenu;
}
