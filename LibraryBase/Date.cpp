#include "Date.h"
#include "prerequisite.h"
#include <chrono>

using std::chrono::system_clock;
using namespace std::literals;

namespace dk
{
	Date::Date()
		: m_year(1970), m_month(1), m_day(1)
	{
	}

	Date::Date(Year year, Month month, Day day)
		: m_year(year), m_month(month), m_day(day)
	{
	}

	time_t Date::to_time_t() const
	{
		std::tm tm = {};
		tm.tm_year = m_year - 1900;
		tm.tm_mon = m_month - 1;
		tm.tm_mday = m_day;
		return system_clock::to_time_t(system_clock::from_time_t(std::mktime(&tm)));
	}
	std::string Date::toDateString() const
	{
		char buffer[11]; // YYYY-MM-DD
		std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", m_year, m_month, m_day);
		return std::string(buffer);
	}
}