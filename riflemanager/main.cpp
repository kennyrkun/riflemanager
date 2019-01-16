#include <SFUI/SFUI.hpp>
#include <SFUI/Theme.hpp>

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>

// This is an example program uitlising every control in SFUI.
// It is intended to be reviewed, and used as a learning tool
// as well as quickly previewing any changes made SFUI itself.

// Resources are not provided twice here, if you are missing
// resources copy the or symlink the folder in SFUI.

void openTeamsAttendanceMenu(SFUI::Menu* menu)
{
	SFUI::FormLayout* form = menu->addFormLayout();
	SFUI::VerticalBoxLayout* vbox = menu->addVerticalBoxLayout();

	for (size_t i = 0; i < 10; i++)
	{
		SFUI::CheckBox* cbox = new SFUI::CheckBox;

		form->addRow("Team Member " + std::to_string((i + 1)), cbox);
	}

	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();
	menu->addHorizontalBoxLayout();

	SFUI::FormLayout* form2 = vbox->addFormLayout();

	form2->addLabel("Present: 6");
	form2->addLabel("Absent: 4");

	form2->addButton("Submit");
}

void openRifleSignOut(SFUI::Menu* menu)
{
	SFUI::FormLayout* form = menu->addFormLayout();

	SFUI::InputBox* nameBox = new SFUI::InputBox;
	SFUI::InputBox* snBox = new SFUI::InputBox;

	form->addRow("Name: ", nameBox);
	form->addRow("Rifle S/N: ", snBox);
	form->addButton("Sign out.");
}

class RifleManager
{
public:
	void openRifleReturn(SFUI::Menu* menu)
	{
		SFUI::FormLayout* form = menu->addFormLayout();

		form->addLabel("Rifles Out:");

		for (size_t i = 0; i < riflesOut.size(); i++)
			form->addButton(std::to_string(riflesOut[0].first) + "/" + riflesOut[1].second);
	}

	void openRifleList(SFUI::Menu* menu)
	{
		SFUI::FormLayout* form = menu->addFormLayout();

		form->addLabel("Rifles Out:");

		for (size_t i = 0; i < riflesOut.size(); i++)
			form->addButton(std::to_string(riflesOut[i].first) + "/" + riflesOut[i].second);
	}

	void openRiflesOut(SFUI::Menu* menu)
	{

	}

	void loadRifleData()
	{
		{
			// load rifle data
			SettingsParser sp("./resources/rifles.dat");

			std::ifstream readIndex("./resources/rifles.dat", std::ios::in);
			std::string line; // each line of index.dat;
			int loopi(0);
			while (std::getline(readIndex, line))
			{
				std::string key = line.substr(0, line.find_first_of('=') - 2);
				std::string type_ = line.substr(line.find_first_of('=') + 2);
				std::string type = type_.substr(0, type_.find_first_of(','));
				std::string status = line.substr(line.find_first_of(',') + 2);

				std::pair<int, std::pair<std::string, std::string>> rifle;

				rifle.first = std::stoi(key);
				rifle.second.first = type;
				rifle.second.second = status;

				rifles.push_back(rifle);
			}
		}
		{
			// load rifles out
			SettingsParser sp("./resources/rifles_out.dat");

			std::ifstream readIndex("./resources/rifles_out.dat", std::ios::in);
			std::string line; // each line of index.dat;
			int loopi(0);
			while (std::getline(readIndex, line))
			{
				std::string id = line.substr(0, line.find_first_of('=') - 1);
				std::string name = line.substr(line.find_first_of('=') + 1, line.length());

				std::pair<int, std::string> rifle;

				rifle.first = std::stoi(id);
				rifle.second = name;

				riflesOut.push_back(rifle);
			}
		}
	}

	// rifle id
	// rifle type,
	// rifle status
	std::vector<std::pair<int, std::pair<std::string, std::string>>> rifles;

	// rifle id
	// rifle status
	std::vector<std::pair<int, std::string>> riflesOut;
};

int main()
{
	SFUI::Theme::loadFont("resources/interface/tahoma.ttf");
	SFUI::Theme::loadTexture("resources/interface/square.png");
	SFUI::Theme::textCharacterSize = 11;
	SFUI::Theme::click.textColor = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorHover = SFUI::Theme::hexToRgb("#191B18");
	SFUI::Theme::click.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColor = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::input.textColorHover = SFUI::Theme::hexToRgb("#CC7A00");
	SFUI::Theme::input.textColorFocus = SFUI::Theme::hexToRgb("#000000");
	SFUI::Theme::windowBgColor = SFUI::Theme::hexToRgb("#dddbde");
	SFUI::Theme::PADDING = 2.f;

	sf::RenderWindow window(sf::VideoMode(300, 500), "KUNS");

	SFUI::Menu* menu = new SFUI::Menu(window);
	menu->setPosition(sf::Vector2f(20, 50));

	RifleManager rifleman;

	rifleman.loadRifleData();
	rifleman.openRifleList(menu);

	sf::RectangleShape navbar(sf::Vector2f(window.getSize().x, 40));
	navbar.setFillColor(sf::Color(93, 138, 168));

	// Attendance
	// Rifle Sign Out
	// Rifle Return
	sf::Text navbarText("Rifle Return", SFUI::Theme::getFont());
	navbarText.setPosition(sf::Vector2f(40, 0));

	sf::RectangleShape arrowLeft(sf::Vector2f(25, 25));
	sf::Texture arrowLeft_tex;
	arrowLeft_tex.loadFromFile("resources/interface/arrow_left.png");
	arrowLeft_tex.setSmooth(true);
	arrowLeft.setTexture(&arrowLeft_tex);
	arrowLeft.setPosition(sf::Vector2f(7.5f, 7.5f));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				window.close();
			else if (event.type == sf::Event::EventType::Resized)
			{
				std::cout << "new width: " << event.size.width << std::endl;
				std::cout << "new height: " << event.size.height << std::endl;

				sf::FloatRect visibleArea(0.0f, 0.0f, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			else if (event.type == sf::Event::EventType::LostFocus)
			{
				std::cout << "lost focus" << std::endl;
			}

			menu->onEvent(event);
		}

		window.clear(SFUI::Theme::windowBgColor);

		if (navbarText.getString() == "Rifle Return")
		{
			for (int i = 0; i < rifleman.riflesOut.size(); i++)
			{
				sf::CircleShape closeButton(8);
				closeButton.setFillColor(sf::Color::Red);
				sf::Vector2f pos = menu->getPosition();

				pos.x -= 8;
				pos.y += 30 + (i * 28);

				closeButton.setPosition(pos);

				sf::Texture tx;
				tx.loadFromFile("resources/interface/close_button.png");
				tx.setSmooth(true);

//				closeButton.setTexture(&tx);

				window.draw(closeButton);
			}
		}

		window.draw(navbar);
		window.draw(navbarText);
		window.draw(arrowLeft);
		window.draw(*menu);

		window.display();
	}

	return 0;
}
