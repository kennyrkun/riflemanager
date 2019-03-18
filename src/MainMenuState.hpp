#ifndef MAIN_MENU_STATE_HPP
#define MAIN_MENU_STATE_HPP

#include "AppState.hpp"

#include "RifleManager.hpp"

#include <SFUI/Layouts/Menu.hpp>

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

	SFUI::Menu* menu;

	SFUI::Menu* buildMainMenu();
};

#endif // !MAIN_MENU_STATE_HPP