#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

const std::string getDateTimestamp()
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];
	localtime_s(&timeinfo, &now);
	strftime(buf, sizeof(buf), "%F.%H-%M-%S", &timeinfo);

	return buf;
}

std::string write_dir = "./rfman_log/" + getDateTimestamp() + ".log";

const std::string getTimestamp()
{
	time_t     now = time(0);
	struct tm  timeinfo;
	char       buf[80];
	localtime_s(&timeinfo, &now);
	strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);

	return buf;
}

void writeLog(const std::string output)
{
	std::ofstream log((write_dir), std::ios::app);

	if (log.is_open())
	{
		log << output << std::endl;

		log.close();

		if (log.bad())
		{
			std::cerr << "failed to write to log!" << std::endl;
		}
	}
	else
	{
		std::cerr << "/n" << "unable to open log file for writing!" << std::endl;
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
			std::cout << output << std::endl;

		writeLog(output);
	}

	void ERROR(std::string output, bool silent)
	{
		output = getTimestamp() + " ERROR: " + output;
		
		if (!silent)
			std::cerr << output << std::endl;

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
