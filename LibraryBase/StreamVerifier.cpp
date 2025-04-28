#include "StreamVerifier.h"
#include "Stream.h"

namespace dk
{
	void OStreamVerifier::verify(Stream& stream, std::size_t size)
	{
		if (stream.getWritableSize() < size)
		{
			stream.m_buf->allocate(stream.size() + size);
		}
	}
	void IStreamVerifier::verify(Stream& stream, std::size_t size)
	{
		if (stream.getReadableSize() < size)
		{
			throw StreamException("Stream buffer underflow");
		}
	}
}