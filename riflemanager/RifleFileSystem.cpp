#include "RifleFileSystem.hpp"

#include "Logger.hpp"
#include <SFML/System/Clock.hpp>

#include <filesystem>

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

bool rfs::newRifle(rifle::serial serial)
{
	if (doesRifleExist(serial))
	{
		logger::ERROR("cannot create new rifle: rifle " + std::to_string(serial) + " already exists.");
		return false;
	}

	// create the rifle
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
