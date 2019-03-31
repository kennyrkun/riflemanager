#include "AdminState.hpp"
#include "DebugState.hpp"
#include "SettingsState.hpp"
#include "NewRifleState.hpp"
#include "RifleInformationState.hpp"

#include "Logger.hpp"
#include "Password.hpp"

#include <SFUI/SFUI.hpp>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

enum Login
{
	FORGOT_PASSWORD,
	LOGIN,

	FIRST_TIME_ADMIN,

	BACK_L,
};

enum Admin
{
	SETTINGS,
//	DEBUG,
	CHECKOUT_ALL_RIFLES,
	RETURN_ALL_RIFLES,
	ADD_NEW_RIFLE,
	EDIT_RIFLE,
	LOGOUT,

	BACK_A
};

void AdminState::Init(AppEngine* app)
{
	logger::INFO("AdminState Init.");

	this->app = app;

	if (app->adminLoggedIn)
		menu = buildMainMenu();
	else
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
//			case Admin::DEBUG:
//				app->PushState(new DebugState);
//				break;
			case Admin::BACK_A:
				app->PopState();
				break;
			case Admin::CHECKOUT_ALL_RIFLES:
				app->rm.checkoutAllRifles();
				break;
			case Admin::RETURN_ALL_RIFLES:
				app->rm.returnAllRifles();
				break;
			case Admin::ADD_NEW_RIFLE:
				app->PushState(new NewRifleState);
				break;
			case Admin::EDIT_RIFLE:
				app->PushState(new RifleInformationState);
				break;
			case Admin::LOGOUT:
				app->adminLoggedIn = false;
				delete menu;
				menu = buildAdminLogin();
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
					app->adminLoggedIn = true;

					delete menu;
					menu = buildMainMenu();
				}
				else
					logger::ERROR("Wrong password given for " + username);

				break;
			}
			case Login::FIRST_TIME_ADMIN:
			{
				// TODO: visually notify
				if (passwordBox->getText() != passwordConfirmBox->getText())
				{
					logger::ERROR("Passwords do not match.");
				}
				else
				{
					fs::create_directory("./resources/admin");

					{
						std::ofstream createPassowrds("./resources/admin/" + usernameBox->getText().toAnsiString() + ".dat");

						if (!createPassowrds.is_open())
						{
							logger::ERROR("Failed to create data file");
							break;
						}
					}

					// TODO: check for failure
					password::generateHashedPassword(usernameBox->getText().toAnsiString(), passwordBox->getText().toAnsiString());

					delete menu;
					menu = buildAdminLogin();
				}

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

	if (!fs::exists("./resources/admin"))
	{
		newMenu->addLabel("First Time Admin Setup");

		newMenu->addLabel("Username");
		usernameBox = new SFUI::InputBox();
		newMenu->add(usernameBox);

		// TODO: PasswordInputBox
		// hides the input and shows dots instead
		newMenu->addLabel("Password");
		passwordBox = new SFUI::InputBox();
		newMenu->add(passwordBox);
		
		// TODO: confirm password
		newMenu->addLabel("Confirm Password");
		passwordConfirmBox = new SFUI::InputBox();
		newMenu->add(passwordConfirmBox);

		newMenu->addButton("Create", Login::FIRST_TIME_ADMIN);
	}
	else
	{
		newMenu->addLabel("Administrator Login");

		usernameBox = new SFUI::InputBox();
		newMenu->add(usernameBox);

		// TODO: PasswordInputBox
		// hides the input and shows dots instead
		passwordBox = new SFUI::InputBox();
		newMenu->add(passwordBox);

		// TODO: make this usable
		newMenu->addLabel("Forgot password?");

		newMenu->addHorizontalBoxLayout();

		newMenu->addButton("Login", Login::LOGIN);
		newMenu->addButton("Back", Login::BACK_L);
	}

	return newMenu;
}

SFUI::Menu* AdminState::buildMainMenu()
{
	menuState = MenuState::MAIN_MENU;

	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(8, 10));

	newMenu->addLabel("Administration");

	newMenu->addButton("Settings", Admin::SETTINGS);
//	I don't really think the Debug state needs to exist just yet.
//	newMenu->addButton("Debug", Admin::DEBUG);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Checkout all Rifles", Admin::CHECKOUT_ALL_RIFLES);
	newMenu->addButton("Return all Rifles", Admin::RETURN_ALL_RIFLES);
	newMenu->addButton("Add Rifle", Admin::ADD_NEW_RIFLE);
	newMenu->addButton("Edit Rifle...", Admin::EDIT_RIFLE);

#ifndef PLATFORM_TOUCH
	newMenu->addHorizontalBoxLayout();

	SFUI::HorizontalBoxLayout* hbox = newMenu->addHorizontalBoxLayout();
	hbox->addButton("Back", Admin::BACK_A);
	hbox->addButton("Logout", Admin::LOGOUT);
#endif

	return newMenu;
}
