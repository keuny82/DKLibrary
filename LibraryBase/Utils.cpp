#include "Utils.h"
#include <time.h>
#include <sstream>
#include <iomanip>


namespace dk
{
	std::string getTimeStringMs(TimePoint currentTime)
	{
		char timeValue[24] = { 0, };
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count() % 1000;
		std::tm tm;
		auto tt = std::chrono::system_clock::to_time_t(currentTime);
		::localtime_s(&tm, &tt);
		strftime(timeValue, 20, "%F %T", &tm);
		snprintf(timeValue + 19, 5, ".%03u", (uint32_t)millis);
		return timeValue;
	}

	//--------------------------------------------------------------------------------
	std::string	getTimeString(TimePoint currentTime)
	{
		return getTimeStringMs(currentTime).substr(0, 19);
	}

	std::string getFileTimestring(TimePoint timePoint)
	{
		std::string time = getTimeString(timePoint);
		std::string out;
		out.reserve(time.size());
		for (size_t i = 0; i < time.size(); ++i)
		{
			if (time[i] == ':')
			{
				out += '-';
			}
			else if (time[i] == ' ')
			{
				out += '_';
			}
			else
			{
				out += time[i];
			}
		}
		return out;
	}

	std::string getLogPrefix(const char* func, const char* file, int line)
	{
		std::string fileName = file;
		fileName = fileName.substr(fileName.find_last_of("/\\"));

		int threadID = std::hash<std::thread::id>{}(std::this_thread::get_id()) % 65536;

		std::stringstream ss;
		ss << "[" << std::setfill('0') << std::setw(4) << threadID << "] "
			<< "[" << fileName << "] :" << std::dec << line << " : " << func << "]";

		return ss.str();
	}
}