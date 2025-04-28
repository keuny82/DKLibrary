#include "SteadyTime.h"
#include "KTime.h"
#include "Stream.h"

using namespace std::chrono;
using namespace std::literals;

namespace dk
{
	SteadyTime::SteadyTime()
	{
	}

	SteadyTime::SteadyTime(const Time& time)
		: m_timepoint(steady_clock::now() - (system_clock::now() - time.to_time_point()))
	{
	}

	SteadyTime::SteadyTime(const SteadyTime& time)
		: m_timepoint(time.m_timepoint)
	{
	}

	time_t SteadyTime::to_time_t() const
	{
		auto diff = duration_cast<system_clock::duration>(difference());
		return system_clock::to_time_t(system_clock::now() - diff);
	}
}