#include "AppEngine.hpp"
#include "MainMenuState.hpp"
#include "RifleListState.hpp"
#include "RifleCheckoutState.hpp"

#include <SFUI/SFUI.hpp>

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void MainMenuState::Init(AppEngine* app_)
{
	logger::INFO("RifleListState Init");
	app = app_;

	int padding = 10;

	int sizeX = app->window->getSize().x - (padding * 2);
	// FIXME: this doesn't work completely correctly, but it's close enough for release.
	int sizeY = (app->window->getSize().y / 2) - (padding * 2);

	listButton.setSize(sf::Vector2f(sizeX, sizeY));
	listButton.setFillColor(sf::Color::Blue);
	listButton.setPosition(sf::Vector2f(padding, padding));

	checkoutButton.setSize(sf::Vector2f(sizeX, sizeY));
	checkoutButton.setFillColor(sf::Color::Red);
	checkoutButton.setPosition(sf::Vector2f(padding, sizeY + (padding * 2)));

	logger::INFO("RifleListState ready");
}

void MainMenuState::Cleanup()
{
	logger::INFO("Cleaning up RifleListState");

	logger::INFO("RifleListState Cleanup");
}

void MainMenuState::Pause()
{
	logger::INFO("RifleListState paused");
}

void MainMenuState::Resume()
{
	logger::INFO("RifleListState resumed");
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

			int padding = 10;

			int sizeX = app->window->getSize().x - (padding * 2);
			int sizeY = (app->window->getSize().y / 2) - (padding * 2);

			listButton.setSize(sf::Vector2f(sizeX, sizeY));
			listButton.setPosition(sf::Vector2f(padding, padding));

			checkoutButton.setSize(sf::Vector2f(sizeX, sizeY));
			checkoutButton.setPosition(sf::Vector2f(padding, sizeY + (padding * 2)));
		}
		else if (event.type == sf::Event::EventType::MouseButtonReleased)
		{
			if (mouseIsOver(listButton))
				app->ChangeState(new RifleListState);
			else if (mouseIsOver(checkoutButton))
				app->ChangeState(new RifleCheckoutState);
		}
	}
}

void MainMenuState::Update()
{
}

void MainMenuState::Draw()
{
	app->window->clear(SFUI::Theme::windowBgColor);
	
	app->window->draw(listButton);
	app->window->draw(checkoutButton);

	app->window->display();
}

bool MainMenuState::mouseIsOver(const sf::Shape &object)
{
	if (object.getGlobalBounds().contains(app->window->mapPixelToCoords(sf::Mouse::getPosition(*app->window))))
		return true;
	else
		return false;
}
