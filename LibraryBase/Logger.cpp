#include "Logger.h"
#include "Utils.h"

namespace dk
{
	Logger::Callback Logger::m_callback = nullptr;

	void Logger::log(dk::log_level level, const char* func, const char* file, int line, const std::string& message)
	{
		if (m_callback)
		{
			m_callback(level, getLogPrefix(func, file, line) + "\t" + message);
		}
	}
}