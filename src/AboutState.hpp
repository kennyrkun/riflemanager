#ifndef ABOUT_STATE_HPP
#define ABOUT_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>

class AboutState : public AppState
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

#endif // !ABOUT_STATE_HPP