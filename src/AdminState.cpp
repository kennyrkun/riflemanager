#include "AdminState.hpp"
#include "DebugState.hpp"
#include "SettingsState.hpp"

#include "Logger.hpp"
#include "Password.hpp"

#include <SFUI/SFUI.hpp>

enum Login
{
	BACK_L,
	FORGOT_PASSWORD,
	LOGIN,
};

enum Admin
{
	BACK_A,
	DEBUG,
	SETTINGS,
	CHECKOUT_ALL_RIFLES,
	RETURN_ALL_RIFLES
};

void AdminState::Init(AppEngine* app)
{
	logger::INFO("AdminState Init.");

	this->app = app;

	menu = buildAdminLogin();

	logger::INFO("AdminState Ready.");
}

void AdminState::Cleanup()
{
	logger::INFO("Cleaing up AdminState.");

	delete menu;

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
		else if (event.type == sf::Event::EventType::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Key::Escape)
				app->PopState();
		}

		int id = menu->onEvent(event);

		if (menuState == MenuState::MAIN_MENU)
		{
			switch (id)
			{
			case Admin::SETTINGS:
				app->PushState(new SettingsState);
				break;
			case Admin::DEBUG:
				app->PushState(new DebugState);
				break;
			case Admin::BACK_A:
				app->PopState();
				break;
			case Admin::CHECKOUT_ALL_RIFLES:
				app->rm.checkoutAllRifles();
				break;
			case Admin::RETURN_ALL_RIFLES:
				app->rm.returnAllRifles();
				break;
			default:
				break;
			}
		}
		else if (menuState == MenuState::LOGIN_MENU)
		{
			switch (id)
			{
			case Login::LOGIN:
			{
				std::string username = usernameBox->getText().toAnsiString();
				std::string password = passwordBox->getText().toAnsiString();

				if (password::validatePassword(username, password))
				{
					delete menu;
					menu = buildMainMenu();
				}
				else
					logger::ERROR("Wrong password given for " + username);

				break;
			}
			case Login::BACK_L:
				app->PopState();
				break;
			default:
				break;
			}
		}
		else
		{
			logger::ERROR("No Menu state is active!");
		}
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

SFUI::Menu* AdminState::buildAdminLogin()
{
	menuState = MenuState::LOGIN_MENU;

	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(8, 10));

	newMenu->addLabel("Administrator Login");

	usernameBox = new SFUI::InputBox();
	newMenu->add(usernameBox);
	
	// TODO: PasswordInputBox
	// hides the input and shows dots instead
	passwordBox = new SFUI::InputBox();
	newMenu->add(passwordBox);

	newMenu->addLabel("Forgot password?");

	newMenu->addHorizontalBoxLayout();
	
	newMenu->addButton("Login", Login::LOGIN);
	newMenu->addButton("Back", Login::BACK_L);

	return newMenu;
}

SFUI::Menu* AdminState::buildMainMenu()
{
	menuState = MenuState::MAIN_MENU;

	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(8, 10));

	newMenu->addLabel("AdminState");

	newMenu->addButton("Settings", Admin::SETTINGS);
	newMenu->addButton("Debug", Admin::DEBUG);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Checkout all Rifles", Admin::CHECKOUT_ALL_RIFLES);
	newMenu->addButton("Return all Rifles", Admin::RETURN_ALL_RIFLES);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Add Rifle");
	newMenu->addButton("Remove Rifle");

#ifndef PLATFORM_TOUCH
	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", Admin::BACK_A);
#endif

	return newMenu;
}
