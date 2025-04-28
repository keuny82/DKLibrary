#pragma once

#include <functional>
#include <string>

namespace dk
{
	enum log_level
	{
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	class Logger
	{
	private:
		using Callback = std::function<void(dk::log_level, const std::string&)>;

	public:
		static void setCallback(Callback callback)
		{
			m_callback = callback;
		}

		static void log(dk::log_level level, const char* func, const char* file, int line, const std::string& message);		

	private:
		static Callback m_callback;
	};
}

#define LOG(level, message) dk::Logger::log(level, __func__, __FILE__, __LINE__, message)