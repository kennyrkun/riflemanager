#include "RifleManager.hpp"

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>

void RifleManager::loadRifleData()
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
			// TODO: this might need should be - 2
			std::string id = line.substr(0, line.find_first_of('=') - 1);
			std::string name = line.substr(line.find_first_of('=') + 2, line.length());

			std::pair<int, std::string> rifle;

			rifle.first = std::stoi(id);
			rifle.second = name;

			riflesOut.push_back(rifle);
		}
	}
}