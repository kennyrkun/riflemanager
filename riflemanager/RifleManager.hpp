#ifndef RIFLE_MANAGER_HPP
#define RIFLE_MANAGER_HPP

#include "Rifle.hpp"

#include <vector>
#include <string>

// TODO: rifle manager does not need to be a class.

class RifleManager
{
public:
	void loadRifleData();

	void checkoutRifle(rifle::serial serial, std::string user);

	// TODO: return rifle by user
//	void returnRifle(std::string user);
	bool returnRifle(rifle::serial serial);

	rifle::Info getRifleInfo(rifle::serial serial);

	bool verifyRifleSerial(std::string serial);

	bool isRifleOut(rifle::serial serial);

	std::vector<rifle::serial> rifles;

	// rifle id
	// rifle type,
	// rifle status
	//std::vector<std::pair<int, std::pair<std::string, std::string>>> rifles;

	// rifle id
	// rifle user
	//std::vector<std::pair<int, std::string>> riflesOut;
};

#endif // !RIFLE_MANAGER_HPP
