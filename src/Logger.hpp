#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

//TODO: logger using format of std::cout

namespace logger
{
//	bool log_using_buffer = false;

	// TODO: have cl_debug override silents
	// so that when debug is enabled, all logs
	// are shown in the active console.

	void DEBUG(std::string output, bool silent = false);

	void INFO(std::string output, bool silent = false);

	void WARNING(std::string output, bool silent = false);

	void ERROR(std::string output, bool silent = false);

	void CUSTOM(const std::string& type, std::string output, bool silent = false);

	void LINE_BREAK();

	void setOutputDir(const std::string& dir);
	void setOutputDir(const std::string& dir, const std::string& filename);
}

#endif // !LOGGER_HPP 
