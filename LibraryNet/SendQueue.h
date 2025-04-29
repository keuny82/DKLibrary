#pragma once

#include "Stream.h"
#include <vector>
#include <deque>
#include <boost/asio/buffer.hpp>

namespace dk
{
	using ConstBufferSequence = std::vector<boost::asio::const_buffer>;
	class SendQueue
	{
	public:
		SendQueue();
		~SendQueue() = default;
		void clear();
		void push(const char* data, size_t size);
		void push(const std::string& data);
		void push(const Stream& stream);
		void consume(size_t size);
		ConstBufferSequence buffers() const;
		size_t size() const;
		bool empty() const;
		const char* front() const;
		size_t frontSize() const;
		void popFront(size_t size);
	
	private:
		using StreamQueue = std::deque<Stream>;
		StreamQueue m_StreamQueue;
	};

}