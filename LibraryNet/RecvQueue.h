#pragma once

#include "Stream.h"
#include <vector>
#include <deque>
#include <boost/asio/buffer.hpp>


namespace dk
{
	using MutableBufferSequence = std::vector<boost::asio::mutable_buffer>;
	class RecvQueue
	{
	public:
		RecvQueue();
		~RecvQueue() = default;
		void clear();
		void prepare(size_t size);
		MutableBufferSequence buffers() const;
		size_t getReadableSize() const;
		size_t getWritableSize() const;

		void commit(size_t size);
		void peek(void* data, size_t size) const;
		void pop(size_t size);
		bool detachable(size_t size) const;
		Stream detach();
		
	public:
		enum { DEFAULT_BUF_SIZE = 4096 };
	private:
		using StreamQueue = std::deque<Stream>;
		StreamQueue m_StreamQueue;
	};
}