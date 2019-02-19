#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>

#include "RifleManager.hpp"

// FIXME: resizing too small causes infinite (or so it seems) hang

class MainMenuState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;

	sf::RectangleShape listButton;
	sf::RectangleShape checkoutButton;

	bool mouseIsOver(const sf::Shape &object);
};

#endif // !MAIN_MENU_STATE_HPP