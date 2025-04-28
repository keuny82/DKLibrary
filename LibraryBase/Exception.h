#pragma once

#include <exception>
#include <string>

namespace dk
{
	class Exception : public std::exception
	{
	public:
		explicit Exception(const std::string& message = {}) : m_message(message)
		{
		}
		virtual ~Exception() noexcept override = default;
		virtual const char* what() const noexcept override
		{
			return m_message.c_str();
		}

	protected:
		std::string m_message;
	};
}