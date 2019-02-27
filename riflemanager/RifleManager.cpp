#include "RifleManager.hpp"

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

void RifleManager::loadRifleData()
{
	logger::INFO("loading rifle data");

	{
		// load rifle data
		std::ifstream readIndex("./resources/rifleinventory/rifles.dat", std::ios::in);
		std::string line; // each line of index.dat;

		while (std::getline(readIndex, line))
		{
			try
			{
				logger::INFO("adding " + line);

				serial rifleSerial = std::stoi(line);
				rifles.push_back(rifleSerial);
			}
			catch (std::exception& e)
			{
				logger::ERROR("failed to convert rifle serial to integer (malformed data file?)");
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
	else
	{
		logger::INFO("rifle " + std::to_string(serial) + " does not exist!");
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
	else
	{
		logger::INFO("rifle " + std::to_string(serial) + " does not exist!");
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
		logger::INFO("failed to convert serial to integer");
		return false;
	}

	if (serial.length() != 6)
	{
		logger::INFO("serial not six characters");
		return false;
	}

	return true;
}

bool RifleManager::isRifleOut(serial serial)
{
	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		std::string status;
		sp.get("status", status);

		return status == "out" ? true : false;
	}
	else
	{
		logger::INFO("rifle " + std::to_string(serial) + " does not exist!");
	}
	
	return false;
}

