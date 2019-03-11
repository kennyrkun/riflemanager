#ifndef SETTINGS_STATE_HPP
#define SETTINGS_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>

class SettingsState : public AppState
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
};

#endif // !SETTINGS_STATE_HPP