#ifndef RIFLE_MANAGER_HPP
#define RIFLE_MANAGER_HPP

#include <vector>
#include <string>

typedef signed int serial;

struct RifleInfo
{
	std::string type;
	std::string status;
	std::string notes;
	std::string user;
	serial serial;
};

class RifleManager
{
public:
	void loadRifleData();

	void checkoutRifle(serial serial, std::string user);

	// TODO: return rifle by user
//	void returnRifle(std::string user);
	void returnRifle(serial serial);

	RifleInfo getRifleInfo(serial serial);

	bool verifyRifleSerial(std::string serial);

	bool isRifleOut(serial serial);

	std::vector<serial> rifles;

	// rifle id
	// rifle type,
	// rifle status
	//std::vector<std::pair<int, std::pair<std::string, std::string>>> rifles;

	// rifle id
	// rifle user
	//std::vector<std::pair<int, std::string>> riflesOut;
};

#endif // !RIFLE_MANAGER_HPP
