#pragma once

#include "Logger.h"

#include <cmath>
#include <algorithm>


namespace dk
{
	class StreamBuf
	{
	public:
		explicit StreamBuf(std::size_t size) : m_size(0), m_buffer(nullptr) { allocate(size); }
		~StreamBuf() { deallocate(); }
		operator uint8_t*() const { return getBuffer(); }
		operator char* () const { return reinterpret_cast<char*>(getBuffer()); }
		uint8_t* getBuffer() const { return m_buffer; }
		std::size_t getSize() const { return m_size; }

	private:
		void allocate(std::size_t size)
		{
			if (0 < size)
			{
				m_size = nextPow2(size);
				m_buffer = (m_buffer == nullptr) ? (uint8_t*)::malloc(m_size) : (uint8_t*)std::realloc(m_buffer, m_size);
			}
		}

		void deallocate()
		{
			if (m_size == 0 && m_buffer != nullptr)
			{
				LOG(dk::log_level::FATAL, "HEAP CORRUPTION DETECTED");
			}

			if (0 < m_size && m_buffer != nullptr)
			{
				::free(m_buffer);
				m_buffer = nullptr;
			}
		}

		std::size_t nextPow2(std::size_t size) const
		{
			return (std::size_t)std::pow(2, std::ceil(::log2(std::max<std::size_t>(size, MIN_ALLOC_SIZE))));
		}
	private:
		std::size_t m_size;
		uint8_t* m_buffer;
		enum {	MIN_ALLOC_SIZE = 64	};

		friend class OStreamVerifier;
	};
	
}