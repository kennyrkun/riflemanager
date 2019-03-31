#include "RifleManager.hpp"

#include "SettingsParser.hpp"
#include "Logger.hpp"
#include "RifleFileSystem.hpp"

#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void RifleManager::loadRifleData()
{
	logger::INFO("[RIFLE MANAGER]: loading rifle data");

	rifles = rfs::getRifleList();
}

// in this function, we assume both the name and serial are valid.
void RifleManager::checkoutRifle(rifle::serial serial, std::string user)
{
	logger::INFO("[RIFLE MANAGER]: checking out rifle " + std::to_string(serial) + " for " + user);

	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		sp.set("user", user);
		sp.set("status", "out");
	}
	else
	{
		logger::ERROR("[RIFLE MANAGER]: rifle " + std::to_string(serial) + " does not exist!");
	}
}

bool RifleManager::returnRifle(rifle::serial serial)
{
	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		sp.set("user", "NO+USER");
		sp.set("status", "in");

		return true;
	}
	else
	{
		logger::ERROR("[RIFLE MANAGER]: rifle " + std::to_string(serial) + " does not exist!");

		return false;
	}
}

rifle::Info RifleManager::getRifleInfo(rifle::serial serial)
{
	return rifle::loadInfo(serial);
}

bool RifleManager::verifyRifleSerial(std::string serial)
{
	for (size_t i = 0; i < serial.size(); i++)
		if (!isdigit(serial[i]))
			return false;

	try
	{
		std::stoi(serial);
	}
	catch (std::exception& e)
	{
		logger::ERROR("[RIFLE MANAGER]: failed to convert serial to integer");
		logger::ERROR(e.what());

		return false;
	}

	if (serial.length() != 6)
	{
		logger::ERROR("[RIFLE MANAGER]: serial not six characters");
		return false;
	}

	return true;
}

bool RifleManager::isRifleOut(rifle::serial serial)
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
		logger::ERROR("[RIFLE MANAGER]: rifle " + std::to_string(serial) + " does not exist!");
	}
	
	return false;
}

void RifleManager::checkoutAllRifles()
{
	logger::INFO("checking out all rifles");

	std::vector<rifle::serial> rifles = rfs::getRifleList();

	for (size_t i = 0; i < rifles.size(); i++)
	{
		if (fs::exists("./resources/rifleinventory/" + std::to_string(rifles[i]) + "/info.dat"))
		{
			SettingsParser parser("./resources/rifleinventory/" + std::to_string(rifles[i]) + "/info.dat");
			parser.set("status", "out");
		}
	}
}

void RifleManager::returnAllRifles()
{
	logger::INFO("returning all rifles");

	std::vector<rifle::serial> rifles = rfs::getRifleList();

	for (size_t i = 0; i < rifles.size(); i++)
		if (rfs::doesRifleExist(rifles[i]))
			returnRifle(rifles[i]);
}
