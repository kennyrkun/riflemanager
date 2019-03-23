#include "AppEngine.hpp"
#include "RifleCheckoutState.hpp"
#include "MainMenuState.hpp"

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

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

	name = new SFUI::InputBox(170);
	rifleID = new SerialInputBox(47);

	menu = buildRifleMenu();

	logger::INFO("RifleCheckoutState ready");
}

void RifleCheckoutState::Cleanup()
{
	logger::INFO("Cleaning up RifleCheckoutState");

	delete menu;

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
		else if (event.type == sf::Event::EventType::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Escape)
				app->PopState();
		}

		int id = menu->onEvent(event);

		switch (id)
		{
			case CALLBACK::SIGN_OUT:
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
				else if (app->rm.isRifleOut(std::stoi(rifleID->getText().toAnsiString())))
				{
					logger::ERROR("rifle is already checked out.");
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
					// TODO: we could put a 1 pixel outline box over the invalid areas
					logger::INFO("not ready to sign out rifle.");
				}

				break;
			}
			case CALLBACK::BACK:
			{
				app->PopState();
				return;
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
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("Checkout Rifle");

	SFUI::FormLayout* form = newMenu->addFormLayout();

	// name and rifleid are initialised in init

	form->addRow("Name:", name, CALLBACK::NAME);
	form->addRow("S/N:", rifleID, CALLBACK::RIFLE_ID);
	form->addButton("Sign Out", CALLBACK::SIGN_OUT);

	newMenu->addHorizontalBoxLayout();
	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
