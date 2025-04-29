#include "SendQueue.h"

namespace dk
{
	SendQueue::SendQueue()
	{
	}
	void SendQueue::clear()
	{
		m_StreamQueue.clear();
	}
	void SendQueue::push(const char* data, size_t size)
	{
		m_StreamQueue.emplace_back(data, size);
	}
	void SendQueue::push(const std::string& data)
	{
		m_StreamQueue.emplace_back(data.data(), data.size());
	}
	void SendQueue::push(const Stream& stream)
	{
		m_StreamQueue.push_back(stream);
	}
	void SendQueue::consume(size_t size)
	{
		if (size == 0) return;
		while (0 < size)
		{
			assert(!m_StreamQueue.empty());
			auto& frontStream = m_StreamQueue.front();
			const auto readableSize = std::min<size_t>(frontStream.getReadableSize(), size);
			assert(0 < readableSize);
			frontStream.consume(readableSize);
			size -= readableSize;
			if (frontStream.getReadableSize() == 0)
				m_StreamQueue.pop_front();
		}
	}
}