#include "Rifle.hpp"

#include "SettingsParser.hpp"
#include "Logger.hpp"

#include <filesystem>
#include <string>

namespace fs = std::experimental::filesystem;

rifle::Info rifle::loadInfo(serial serial)
{
	Info info;

	if (fs::exists("./resources/rifleinventory/" + std::to_string(serial)))
	{
		SettingsParser sp("./resources/rifleinventory/" + std::to_string(serial) + "/info.dat");

		sp.get("status", info.status);
		sp.get("notes", info.notes);
		sp.get("user", info.user);
		sp.get("type", info.type);

		info.serial = serial;
	}
	else
	{
		logger::INFO("rifle " + std::to_string(serial) + " does not exist!");

		info.serial = 0;
		info.status = "INVALID RIFLE";
		info.type = "INVALID RIFLE";
		info.user = "INVALID RIFLE";
		info.notes = "INVALID RIFLE";
	}

	return info;
}
