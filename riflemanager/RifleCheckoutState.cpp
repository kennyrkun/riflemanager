#include "AppEngine.hpp"
#include "RifleCheckOutState.hpp"
#include "MainMenuState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

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
	logger::INFO("RifleCheckoutState Init");
	app = app_;

	delete menu;
	menu = buildRifleMenu();

	logger::INFO("RifleCheckoutState ready");
}

void RifleCheckoutState::Cleanup()
{
	logger::INFO("Cleaning up RifleCheckoutState");

	logger::INFO("RifleCheckoutState Cleanup");
}

void RifleCheckoutState::Pause()
{
	logger::INFO("RifleCheckoutState paused");
}

void RifleCheckoutState::Resume()
{
	logger::INFO("RifleCheckoutState resumed");
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
				logger::INFO("sign out");
				logger::INFO("name: " + name->getText().toAnsiString());
				logger::INFO("S/N: " + rifleID->getText().toAnsiString());

				bool readyToSignOut(true);

				if (name->isEmpty())
				{
					logger::ERROR("name must be specified");
					readyToSignOut = false;
				}

				if (rifleID->isEmpty())
				{
					logger::ERROR("rifle id must be specified");
					readyToSignOut = false;
				}
				else if (!app->rm.verifyRifleSerial(rifleID->getText()))
				{
					logger::ERROR("rifle id is not valid");
					readyToSignOut = false;
				}

				if (readyToSignOut)
				{
					logger::INFO("ready to sign out rifle.");

					rifle::serial serial = std::stoi(rifleID->getText().toAnsiString());

					app->rm.checkoutRifle(serial, name->getText());

					name->setText("");
					rifleID->setText("");
				}
				else
				{
					logger::INFO("not ready to sign out rifle.");
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
	rifleID = new SerialInputBox(47);

	form->addRow("Name:", name, CALLBACK::NAME);
	form->addRow("S/N:", rifleID, CALLBACK::RIFLE_ID);
	form->addButton("Sign Out", CALLBACK::SIGN_OUT);

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	menu->addButton("Back", CALLBACK::BACK);

	return menu;
}
