#pragma once

#include "Exception.h"

namespace dk
{
	struct StreamException : public Exception
	{
		StreamException(const std::string& message = "Stream exception") : Exception(message) {};
	};

	class Stream;

	class OStreamVerifier
	{
	public:
		static void verify(Stream& stream, std::size_t size);
	};

	class IStreamVerifier
	{
	public:
		static void verify(Stream& stream, std::size_t size);
	};
}