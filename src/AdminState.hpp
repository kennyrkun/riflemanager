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
	SFUI::Menu* buildFirstTimeAdmin();
	SFUI::Menu* buildChangePassword();

	SFUI::InputBox* usernameBox;
	SFUI::InputBox* passwordBox;
	SFUI::InputBox* passwordConfirmBox;

	enum MenuState
	{
		MAIN,
		LOGIN,
		FIRST_TIME_ADMIN,
		CHANGE_PASSWORD,
	} menuState;

	std::string currentAccountUsername;
};

#endif // !ADMIN_STATE_HPP