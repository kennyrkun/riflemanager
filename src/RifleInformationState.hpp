#ifndef RIFLE_INFORMATION_STATE_HPP
#define RIFLE_INFORMATION_STATE_HPP

#include "AppState.hpp"
#include "RifleFileSystem.hpp"

#include <SFUI/Layouts/Menu.hpp>
#include <SFUI/OptionsBox.hpp>

class RifleInformationState : public AppState
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
	enum MenuState
	{
		PickingRifle,
		EditingRifle
	} menuState;

	AppEngine* app;

	SFUI::Menu* menu;

	SFUI::Menu* buildRiflePicker();
	SFUI::Menu* buildRifleEditor(rifle::serial rifle);

	SFUI::OptionsBox<std::string>* rifleType;

	rifle::serial rifle;
};

#endif // !RIFLE_INFORMATION_STATE_HPP