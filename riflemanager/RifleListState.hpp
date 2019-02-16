#ifndef RIFLE_LIST_STATE_HPP
#define RIFLE_LIST_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>

#include "RifleManager.hpp"

// FIXME: resizing too small causes infinite (or so it seems) hang

class RifleListState : public AppState
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

	SFUI::Menu* buildRifleMenu();

	RifleManager rm;
};

#endif // !RIFLE_LIST_STATE_HPP