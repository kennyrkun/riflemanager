#include "RifleFileSystem.hpp"

#include "Logger.hpp"

#include <SFML/System/Clock.hpp>

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

const std::string RIFLE_PATH = "./resources/rifleinventory/";

bool rfs::doesRifleExist(rifle::serial serial)
{
	return fs::exists(RIFLE_PATH + std::to_string(serial));
}

bool rfs::removeRifle(rifle::serial serial)
{
	if (doesRifleExist(serial))
	{
		logger::ERROR("cannot remove rifle: rifle " + std::to_string(serial) + " does not exist.");
		return false;
	}

	try
	{
		fs::remove_all(RIFLE_PATH + std::to_string(serial));
		logger::INFO("removed rifle " + std::to_string(serial));

		return true;
	}
	catch (std::exception& e)
	{
		logger::ERROR("cannot remove rifle:");
		logger::ERROR(e.what());

		return false;
	}
}

bool rfs::newRifle(rifle::serial serial, const std::string& type)
{
	if (doesRifleExist(serial))
	{
		logger::ERROR("cannot create new rifle: rifle " + std::to_string(serial) + " already exists.");
		return false;
	}

	try
	{
		fs::create_directory("./resources/rifleinventory/" + std::to_string(serial));
	}
	catch (std::exception& e)
	{
		logger::ERROR("Failed to crate new rifle directory:");
		logger::ERROR(e.what());

		return false;
	}

	std::ofstream createInfo("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

	if (createInfo.is_open())
	{
		createInfo << "serial = " << serial << std::endl;
		createInfo << "user = NO+USER" << std::endl;
		createInfo << "type = " << type << std::endl;
		createInfo << "status = in" << std::endl;
	}
	else
	{
		logger::ERROR("Failed to create info for new rifle!");
		return false;
	}

	return true;
}

size_t rfs::getRifleCount()
{
	int rifles = 0;

	for (const auto& entry : fs::directory_iterator(RIFLE_PATH))
		rifles++;

	return rifles;
}

std::vector<rifle::serial> rfs::getRifleList()
{
	sf::Clock timer;

	std::vector<rifle::serial> rifles;

	for (const auto& entry : fs::directory_iterator(RIFLE_PATH))
	{
		if (fs::is_directory(entry))
		{
			std::string path = entry.path().generic_string();
			path.erase(0, RIFLE_PATH.size());

			logger::DEBUG("[RIFLE FILE SYSTEM]: " + path, true);

			try 
			{
				rifle::serial rifle = std::stoi(path);
				rifles.push_back(rifle);
			}
			catch (std::exception& e)
			{
				logger::ERROR("[RIFLE FILE SYSTEM]: failed to add rifle to list.");
				logger::ERROR("[RIFLE FILE SYSTEM]: " + std::string(e.what()));
			}
		}
	}

	logger::DEBUG("Took " + std::to_string(timer.getElapsedTime().asSeconds()) + "s to get rifle list.");

	return rifles;
}
