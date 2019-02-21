#include "AppEngine.hpp"
#include "RifleCheckOutState.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

enum CALLBACK
{
	NAME,
	RIFLE_ID,
	SIGN_OUT,
	BACK,
};

void RifleCheckoutState::Init(AppEngine* app_)
{
	std::cout << "RifleCheckoutState Init" << std::endl;
	app = app_;

	delete menu;
	menu = buildRifleMenu();

	std::cout << "RifleCheckoutState ready" << std::endl;
}

void RifleCheckoutState::Cleanup()
{
	std::cout << "Cleaning up RifleCheckoutState" << std::endl;

	std::cout << "RifleCheckoutState Cleanup" << std::endl;
}

void RifleCheckoutState::Pause()
{
	std::cout << "RifleCheckoutState paused" << std::endl;
}

void RifleCheckoutState::Resume()
{
	std::cout << "RifleCheckoutState resumed" << std::endl;
}

void RifleCheckoutState::HandleEvents()
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

		if (id > 0)
		{
			if (id == CALLBACK::SIGN_OUT)
			{
				std::cout << "sign out" << std::endl;
				std::cout << "name: " << name->getText().toAnsiString() << std::endl;
				std::cout << "S/N: " << rifleID->getText().toAnsiString() << std::endl;

				bool readyToSignOut(true);

				if (name->isEmpty())
				{
					std::cerr << "name must be specified" << std::endl;
					readyToSignOut = false;
				}

				if (rifleID->isEmpty())
				{
					std::cerr << "rifle id must be specified" << std::endl;
					readyToSignOut = false;
				}
				else if (!app->rm.verifyRifleSerial(rifleID->getText()))
				{
					std::cerr << "rifle id is not valid" << std::endl;
					readyToSignOut = false;
				}

				if (readyToSignOut)
				{
					std::cout << "ready to sign out rifle." << std::endl;

					serial serial = std::stoi(rifleID->getText().toAnsiString());

					app->rm.checkoutRifle(serial, name->getText());

					name->setText("");
					rifleID->setText("");
				}
				else
				{
					std::cout << "not ready to sign out rifle." << std::endl;
				}
			}
			else if (id == CALLBACK::BACK)
			{
				app->ChangeState(new MainMenuState);
			}
		}
	}
}

void RifleCheckoutState::Update()
{
}

void RifleCheckoutState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);
	app->window->draw(*menu);
	app->window->display();
}

SFUI::Menu* RifleCheckoutState::buildRifleMenu()
{
	SFUI::Menu* menu = new SFUI::Menu(*app->window);
	menu->setPosition(sf::Vector2f(10, 10));

	menu->addLabel("Checkout Rifle");

	SFUI::FormLayout* form = menu->addFormLayout();

	delete name;
	name = new SFUI::InputBox(170);

	delete rifleID;
	rifleID = new SFUI::InputBox(47);

	form->addRow("Name:", name, CALLBACK::NAME);
	form->addRow("S/N:", rifleID, CALLBACK::RIFLE_ID);
	form->addButton("Sign Out", CALLBACK::SIGN_OUT);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	menu->addButton("Back", CALLBACK::BACK);

	return menu;
}
