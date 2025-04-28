#include "KTime.h"
#include <iomanip>
#include <sstream>



namespace dk
{
	Time::Time()
		: m_time(0)
	{
	}
	Time::Time(time_t t)
		: m_time(std::max<time_t>(t, 0))
	{
	}
	Time::Time(const Time& t)
		: m_time(t.m_time)
	{
	}
	Time::Time(const std::chrono::system_clock::time_point& tp)
	{
		Time(std::chrono::system_clock::to_time_t(tp));
	}
	Time::Time(const std::tm& tm)
		: Time(::mktime(const_cast<std::tm*>(&tm)))
	{
	}
	Time::Time(const std::string& timeString)
		: Time(to_tm(timeString))
	{
	}	
	std::tm Time::to_tm(time_t time)
	{
		std::tm tm = {};
		::localtime_s(&tm, &time);
		return tm;
	}
}