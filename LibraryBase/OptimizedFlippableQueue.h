#pragma once
#include <deque>
#include <mutex>
#include <atomic>
#include <memory>
#include <array>

namespace dk
{
	template <typename T>
	class OptimizedFlippableQueueT
	{
	public:
		using Queue = std::deque<T>;
		OptimizedFlippableQueueT() : m_pos(0) {}
		~OptimizedFlippableQueueT() {}

		// 큐를 전환 (use lock)
		Queue& flip()
		{
			std::lock_guard<std::mutex> guard(m_lock);
			return flipWeak();
		}

		// 큐를 전환
		Queue& flipWeak()
		{
			m_pos.store(!m_pos, std::memory_order_relaxed);
			return m_queues[!m_pos];
		}

		// 데이터를 큐에 추가 (use lock)
		template<typename... TS>
		void push(TS&&... params)
		{
			std::lock_guard<std::mutex> guard(m_lock);
			pushWeak(std::forward<TS>(params)...);
		}

		// 데이터를 큐에 추가
		template<typename... TS>
		void pushWeak(TS&&... params)
		{
			m_queues[m_pos.load(std::memory_order_relaxed)].emplace_back(std::forward<TS>(params)...);
		}

		bool empty() const
		{		
			return m_queues[m_pos.load(std::memory_order_relaxed)].empty();
		}

	private:
		mutable std::mutex m_lock;
		std::atomic<size_t> m_pos;
		std::array<Queue, 2> m_queues;
	};
}
