#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <thread>

namespace dk
{
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	std::string getTimeStringMs(TimePoint currentTime = std::chrono::system_clock::now());
	std::string getTimeString(TimePoint currentTime = std::chrono::system_clock::now());
	std::string getFileTimestring(TimePoint timePoint);	

	std::string getLogPrefix(const char* func, const char* file, int line);
}