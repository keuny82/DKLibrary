#pragma once

#include <chrono>
#include <ostream>
#include <sstream>
#include <iomanip>

namespace dk
{
	class Time
	{
	public:
		Time();
		explicit Time(time_t time);
		Time(const Time& t);
		explicit Time(const std::chrono::system_clock::time_point& tp);
		explicit Time(const std::string& timeString);
		time_t to_time_t() const { return m_time; }
		std::chrono::system_clock::time_point to_time_point() const
		{
			return std::chrono::system_clock::from_time_t(m_time);
		}
		std::tm to_tm() const
		{
			return to_tm(m_time);
		}
		static std::tm to_tm(time_t time);
		static Time now()
		{
			return Time(::time(nullptr));
		}
		bool operator<=(const Time& other) const
		{
			return !(other.m_time < m_time);
		}
		bool operator<(const Time& other) const
		{
			return m_time < other.m_time;
		}
		std::string toString() const
		{
			std::tm tm = to_tm();
			char buffer[20]; // YYYY-MM-DD HH:MM:SS
			std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
			return std::string(buffer);
		}

	private:
		explicit Time(const std::tm& tm);
		static std::tm to_tm(const std::string& timeString)
		{
			std::tm tm = {};
			std::stringstream ss(timeString);
			ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
			return tm;
		}

	private:
		time_t m_time = 0;
	};
}