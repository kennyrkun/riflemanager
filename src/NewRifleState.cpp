#include "NewRifleState.hpp"

#include "Logger.hpp"
#include "RifleFileSystem.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	SUBMIT,
	BACK,
};

void NewRifleState::Init(AppEngine* app)
{
	logger::INFO("NewRifleState Init.");

	this->app = app;

	menu = buildMainMenu();

	logger::INFO("NewRifleState Ready.");
}

void NewRifleState::Cleanup()
{
	logger::INFO("Cleaing up NewRifleState.");
	logger::INFO("NewRifleState cleaned up.");
}

void NewRifleState::Pause()
{
	logger::INFO("NewRifleState paused.");
}

void NewRifleState::Resume()
{
	logger::INFO("NewRifleState resumed.");
}

void NewRifleState::HandleEvents()
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
		case CALLBACK::SUBMIT:
		{
			if (rifleSerial->isEmpty())
				logger::ERROR("A serial number must be provided.");
			else
			{
				rifle::serial s = std::stoi(rifleSerial->getText().toAnsiString());

				if (rfs::newRifle(s, rifleType->getSelectedValue()))
					rifleSerial->setText(""); // clear it for the next one
			}

			break;
		}
		case CALLBACK::BACK:
			app->PopState();
			break;
		default:
			break;
		}
	}
}

void NewRifleState::Update()
{
}

void NewRifleState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* NewRifleState::buildMainMenu()
{
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("Add New Rifle");

	newMenu->addLabel("Serial:");
	rifleSerial = new SerialInputBox();
	newMenu->add(rifleSerial);

	SFUI::FormLayout* form = newMenu->addFormLayout();

	rifleType = new SFUI::OptionsBox<std::string>;
	rifleType->addItem("Honour Guard", "honorguard");
	rifleType->addItem("Drill", "drill");
	form->addRow("Rifle Type:", rifleType);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Submit", CALLBACK::SUBMIT);
	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
