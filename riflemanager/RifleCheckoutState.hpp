#ifndef RIFLE_CHECKOUT_STATE_HPP
#define RIFLE_CHECKOUT_STATE_HPP

#include "AppState.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/InputBox.hpp>

#include "RifleManager.hpp"
#include "SerialInputBox.hpp"

// FIXME: resizing too small causes infinite (or so it seems) hang

class RifleCheckoutState : public AppState
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

	SFUI::InputBox* name;
	SerialInputBox* rifleID;

	SFUI::Menu* buildRifleMenu();
};

#endif // !RIFLE_CHECKOUT_STATE_HPP