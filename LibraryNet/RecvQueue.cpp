#include "RecvQueue.h"

namespace dk
{
	RecvQueue::RecvQueue()
	{
	}
	void RecvQueue::clear()
	{
		m_StreamQueue.clear();
	}
	void RecvQueue::prepare(size_t size)
	{
		if (m_StreamQueue.empty() || getWritableSize() < size)
		{
			m_StreamQueue.emplace_back(std::max<size_t>(size, DEFAULT_BUF_SIZE));
		}
	}
	MutableBufferSequence RecvQueue::buffers() const
	{
		MutableBufferSequence buffers;
		buffers.reserve(m_StreamQueue.size());
		for (auto& stream : m_StreamQueue)
		{
			size_t writableSize = stream.getWritableSize();
			if (writableSize > 0)
			{
				buffers.emplace_back(stream.getWritableBuffer(), writableSize);
			}
		}
		return buffers;
	}
	size_t RecvQueue::getReadableSize() const
	{
		size_t size = 0;
		for (const auto& stream : m_StreamQueue)
			size += stream.getReadableSize();
		return size;
	}
	size_t RecvQueue::getWritableSize() const
	{
		size_t size = 0;
		for (const auto& stream : m_StreamQueue)
			size += stream.getWritableSize();
		return size;
	}
	void RecvQueue::commit(size_t size)
	{
		for (auto& stream : m_StreamQueue)
		{
			size_t writableSize = stream.getWritableSize();
			if (writableSize >= size)
			{
				stream.commit(size);
				return;
			}
			else
			{
				size -= writableSize;
				stream.commit(writableSize);
			}
		}
	}
	void RecvQueue::peek(void* data, size_t size) const
	{
		assert(size <= getReadableSize());
		for (const auto& stream : m_StreamQueue)
		{
			size_t readableSize = stream.getReadableSize();
			if (readableSize >= size)
			{
				stream.peek(data, size);
				return;
			}
			else
			{
				size -= readableSize;
				stream.peek(data, readableSize);
				data = static_cast<char*>(data) + readableSize;
			}
		}
	}
	void RecvQueue::pop(size_t size)
	{
		assert(size <= getReadableSize());
		size_t readSize = 0;
		for (auto s = m_StreamQueue.begin(); s != m_StreamQueue.end();)
		{
			auto& stream = *s;
			const auto readableSize = std::min<size_t>(stream.getReadableSize(), size - readSize);
			stream.pop(readableSize);
			if (stream.getReadableSize() == 0)
			{
				if (s == m_StreamQueue.end() - 1 && stream.getAllocSize() == DEFAULT_BUF_SIZE)
				{
					stream.reset();
				}
				else
				{
					s = m_StreamQueue.erase(s);
				}
			}
			else
			{
				++s;
			}
			readSize += readableSize;
			if (readSize == size)
				break;
		}
		assert(readSize == size);
	}
	bool RecvQueue::detachable(size_t size) const
	{
		return (m_StreamQueue.front().getReadableSize() == size);
	}
	Stream RecvQueue::detach()
	{
		auto stream = std::move(m_StreamQueue.front());
		m_StreamQueue.pop_front();
		return stream;
	}
}