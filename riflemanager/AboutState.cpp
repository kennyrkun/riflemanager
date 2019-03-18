#include "AboutState.hpp"

#include "Logger.hpp"

#include <SFUI/SFUI.hpp>

enum CALLBACK
{
	BACK,
	SOURCE,
	ISSUES
};

void AboutState::Init(AppEngine* app)
{
	logger::INFO("AboutState Init.");

	this->app = app;

	menu = buildMainMenu();

	logger::INFO("AboutState Ready.");
}

void AboutState::Cleanup()
{
	logger::INFO("Cleaing up AboutState.");
	logger::INFO("AboutState cleaned up.");
}

void AboutState::Pause()
{
	logger::INFO("AboutState paused.");
}

void AboutState::Resume()
{
	logger::INFO("AboutState resumed.");
}

void AboutState::HandleEvents()
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
	}

	int id = menu->onEvent(event);

	switch (id)
	{
	case CALLBACK::BACK:
		app->PopState();
		break;
	case CALLBACK::SOURCE:
		system("start https://github.com/kennyrkun/riflemanager");
		break;
	case CALLBACK::ISSUES:
		system("start https://github.com/kennyrkun/riflemanager/issues");
		break;
	default:
		break;
	}
}

void AboutState::Update()
{
}

void AboutState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);

	app->window->draw(*menu);

	app->window->display();
}

SFUI::Menu* AboutState::buildMainMenu()
{
	SFUI::Menu* newMenu = new SFUI::Menu(*app->window);
	newMenu->setPosition(sf::Vector2f(10, 10));

	newMenu->addLabel("RifleManager\nVersion: " + std::to_string(app->settings.version) + "\nCompiled on " + __DATE__ + " at " + __TIME__);

	#ifdef NDEBUG
	newMenu->addLabel("Compiled in Debug mode.");
	#else
	newMenu->addLabel("Compiled in Release mode.");
	#endif

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Source", CALLBACK::SOURCE);
	newMenu->addButton("Issues", CALLBACK::ISSUES);

	newMenu->addHorizontalBoxLayout();

	newMenu->addButton("Back", CALLBACK::BACK);

	return newMenu;
}
