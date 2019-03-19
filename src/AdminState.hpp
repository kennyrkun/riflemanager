#ifndef ADMIN_STATE_HPP
#define ADMIN_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/InputBox.hpp>

class AdminState : public AppState
{
public:
	void Init(AppEngine* app);
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
	SFUI::Menu* buildAdminLogin();

	SFUI::InputBox* usernameBox;
	SFUI::InputBox* passwordBox;

	enum MenuState
	{
		MAIN_MENU,
		LOGIN_MENU
	} menuState;
};

#endif // !ADMIN_STATE_HPP