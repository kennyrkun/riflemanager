#ifndef RIFLE_MANAGER_HPP
#define RIFLE_MANAGER_HPP

#include <vector>
#include <string>

class RifleManager
{
public:
	void loadRifleData();

	// rifle id
	// rifle type,
	// rifle status
	std::vector<std::pair<int, std::pair<std::string, std::string>>> rifles;

	// rifle id
	// rifle renter
	std::vector<std::pair<int, std::string>> riflesOut;
};

#endif // !RIFLE_MANAGER_HPP
