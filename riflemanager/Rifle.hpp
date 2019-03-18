#ifndef RIFLE_HPP
#define RIFLE_HPP

// TODO: class Rifle : HonorGuardItem, ArmouryItem, TrackableItem

#include <string>

namespace rifle
{
	typedef signed int serial;

	struct Note
	{
		std::string author;
		std::string date;
		std::string content;
	};

	struct Info
	{
		std::string type;
		std::string status;
		std::string notes;
		std::string user;
		serial serviceNo;

		// std::vector<Note> notes;
	};

	Info loadInfo(serial serviceNo);
}

#endif // !RIFLE_HPP