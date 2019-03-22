#ifndef RIFLE_FILE_SYSTEM_HPP
#define RIFLE_FILE_SYSTEM_HPP

#include "Rifle.hpp"

#include <vector>
#include <string>

// rifle file system
namespace rfs
{
	bool doesRifleExist(rifle::serial serial);

	// completely removes the rifle from the filesystem
	bool removeRifle(rifle::serial serial);

	// adds a new rifle to the file system
	bool newRifle(rifle::serial serial, std::string type);

	// returns the amount of rifles in the file system
	size_t getRifleCount();

	// returns a list of rifle serials from the file system
	std::vector<rifle::serial> getRifleList();
}

#endif // !RIFLE_FILE_SYSTEM_HPP
