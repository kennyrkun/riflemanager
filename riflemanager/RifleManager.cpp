#include "RifleManager.hpp"

#include "SettingsParser.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

void RifleManager::loadRifleData()
{
	std::cout << "loading rifle data" << std::endl;

	{
		// load rifle data
		std::ifstream readIndex("./resources/rifleinventory/rifles.dat", std::ios::in);
		std::string line; // each line of index.dat;

		int loopi(0);
		while (std::getline(readIndex, line))
		{
			try
			{
				std::cout << "adding " << line << std::endl;

				serial rifleSerial = std::stoi(line);
				rifles.push_back(rifleSerial);
			}
			catch (std::exception& e)
			{
				std::cerr << "[ERROR]: failed to convert rifle serial to integer (malformed data file?)" << std::endl;
			}
		}
	}
	/*
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
	*/
}

// in this function, we assume both the name and serial are valid.
void RifleManager::checkoutRifle(serial serial, std::string user)
{
	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		sp.set("user", user);
		sp.set("status", "out");
	}
}

void RifleManager::returnRifle(serial serial)
{
	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		sp.set("user", "NO+USER");
		sp.set("status", "in");
	}
}

RifleInfo RifleManager::getRifleInfo(serial serial)
{
	RifleInfo info;

	return info;
}

bool RifleManager::verifyRifleSerial(std::string serial)
{
	for (int i = 0; i < serial.size(); i++)
		if (!isdigit(serial[i]))
			return false;

	try
	{
		std::stoi(serial);
	}
	catch (std::exception& e)
	{
		std::cerr << "failed to convert serial to integer" << std::endl;

		return false;
	}

	if (serial.length() != 6)
	{
		std::cerr << "serial not six characters" << std::endl;
		return false;
	}

	return true;
}

