#ifndef NEW_RIFLE_STATE_HPP
#define NEW_RIFLE_STATE_HPP

#include "AppState.hpp"

#include "SerialInputBox.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/OptionsBox.hpp>

class NewRifleState : public AppState
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

	SerialInputBox* rifleSerial;
	SFUI::OptionsBox<std::string>* rifleType;

	SFUI::Menu* buildMainMenu();
};

#endif // !NEW_RIFLE_STATE_HPP