#pragma once

namespace dk
{
	class Stream;

	struct Serializable
	{
		virtual ~Serializable() = default;
		virtual void serialize(Stream& stream) const = 0;
		virtual void deserialize(Stream& stream) = 0;
	};
}