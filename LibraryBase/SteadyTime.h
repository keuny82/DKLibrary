#pragma once

#include <chrono>
#include <ostream>

namespace dk
{
	class Time;

	class SteadyTime
	{
	public:
		SteadyTime();
		SteadyTime(const Time& time);
		SteadyTime(const SteadyTime& time);

		operator bool() const {
			return m_timepoint.time_since_epoch().count() != 0;
		}

		std::chrono::steady_clock::duration difference() const {
			return std::chrono::steady_clock::now() - m_timepoint;
		}

		time_t timeElapsed() const {
			return std::chrono::duration_cast<std::chrono::duration<time_t>>(difference()).count();
		}

		time_t tiemRemaining() const {
			return -timeElapsed();
		}

		time_t to_time_t() const;

	private:
		std::chrono::steady_clock::time_point m_timepoint;
	};
}
