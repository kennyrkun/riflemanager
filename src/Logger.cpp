#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

#ifdef _WIN32
	#include <Windows.h>
	#undef ERROR
#endif


/*
// TODO: log settings

enum LOG
{
	NoPrint,
	NoWrite,
	NoNewLine,
};

*/

const std::string getDateStamp()
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];

#ifdef _WIN32
	localtime_s(&timeinfo, &now);
#else
	localtime_r(&now, &timeinfo);
#endif

	strftime(buf, sizeof(buf), "%F", &timeinfo);

	return buf;
}

// TODO: replace the - with :
// this uses dashes currently because windows does not allow colons in filenames
const std::string getTimestamp()
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];

#ifdef _WIN32
	localtime_s(&timeinfo, &now);
#else
	localtime_r(&now, &timeinfo);
#endif
	
	strftime(buf, sizeof(buf), "%H-%M-%S", &timeinfo);

	return buf;
}

std::string write_dir = "./rfman_log/" + getDateStamp() + "." + getTimestamp() + ".log";

void writeLog(const std::string output)
{
	std::ofstream log((write_dir), std::ios::app);

	if (log.is_open())
	{
		log << output << std::endl;

		log.close();

		if (log.bad())
		{
			std::cerr << "!!FAILED TO WRITE LOG!!" << std::endl;
		}
	}
	else
	{
#ifdef _WIN32 // TODO: make sure these work on other platforms
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << "LOGGER-ERROR: Unable to open the log file for writing!" << std::endl;
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#elif defined __unix__ 
		std::cout << "\u001b[31m" << "LOGGER-ERROR: Unable to open the log file for writing!" << "\u001b[0m" << std::endl;
#else // anything else
		std::cout << output << std::endl;
#endif
	}
}

namespace logger
{
	void DEBUG(std::string output, bool silent)
	{
		output = getTimestamp() + " DEBUG: " + output;

		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void INFO(std::string output, bool silent)
	{
		output = getTimestamp() + " INFO: " + output;

		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void WARNING(std::string output, bool silent)
	{
		output = getTimestamp() + " WARNING: " + output;

		if (!silent)
		{
#ifdef _WIN32 // TODO: make sure these work on other platforms
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			std::cout << output << std::endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#elif defined __unix__ 
			std::cout << "\u001b[33m" << output << "\u001b[0m" << std::endl;
#else // anything else
			std::cout << output << std::endl;
#endif
		}

		writeLog(output);
	}

	void ERROR(std::string output, bool silent)
	{
		output = getTimestamp() + " ERROR: " + output;
		
		if (!silent)
		{
#ifdef _WIN32 // TODO: make sure these work on other platforms
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			std::cout << output << std::endl;
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#elif defined __unix__ 
			std::cout << "\u001b[31m" << output << "\u001b[0m" << std::endl;
#else // anything else
			std::cout << output << std::endl;
#endif
		}

		writeLog(output);
	}

	void CUSTOM(std::string type, std::string output, bool silent)
	{
		output = getTimestamp() + " " + type + ": " + output;
		
		if (!silent)
			std::cout << output << std::endl;

		writeLog(output);
	}

	void LINE_BREAK()
	{
		std::cout << std::endl;
		writeLog("\n");
	}

	void setOutputDir(const std::string dir)
	{
		write_dir = dir;
	}

	void setOutputDir(const std::string dir, const std::string filename)
	{
		write_dir = dir + "//" + filename + ".log";
	}
}
