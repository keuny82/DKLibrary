#pragma once

#include "prerequisite.h"
#include "Serializable.h"
#include "StreamBuf.h"
#include "StreamVerifier.h"
#include <cstring>
#include <vector>
#include <deque>
#include <unordered_map>
#include <cassert>


namespace dk
{
	class Stream
	{
	public:
		Stream() : Stream(0) {}
		explicit Stream(std::size_t size) : m_buf(new StreamBuf(size)), m_read(0), m_written(0) {}
		// Copy constructor
		Stream(const Stream& other) : m_buf(other.m_buf), m_read(other.m_read), m_written(other.m_written) {}
		// Constructor that initializes the stream with data
		Stream(const void* data, std::size_t size) : Stream(size) { write(data, size); }


		Stream& operator=(const Stream& other)
		{
			if (this != &other)	// 자기 자신이 아닌 경우
			{
				m_buf = other.m_buf;
				m_read = other.m_read;
				m_written = other.m_written;
			}
			return *this;
		}

		uint8_t* getBuffer() const
		{
			return m_buf->getBuffer();
		}

		std::size_t size() const
		{
			return m_written;
		}

		bool empty() const
		{
			return size() == 0;
		}

		void write(const void* data, std::size_t size)
		{
			OStreamVerifier::verify(*this, size);
			::memcpy(getWritableBuffer(), data, size);
			commit(size);
		}
		// 지정된 크키의 빈 공간 확보
		void write(std::size_t size)
		{
			OStreamVerifier::verify(*this, size);
			commit(size);
		}

		void read(void* data, std::size_t size)
		{
			IStreamVerifier::verify(*this, size);
			peek(data, size);	// 데이터를 가져옴 
			pop(size);
		}

		void read(std::size_t size)
		{
			IStreamVerifier::verify(*this, size);
			pop(size);
		}

		uint8_t* getReadableBuffer() const
		{
			return getBuffer() + m_read;
		}

		std::size_t getReadableSize() const
		{
			assert(m_read <= m_written);	// 읽기 위치 체크
			return m_written - m_read;
		}

		uint8_t* getWritableBuffer() const
		{
			return getBuffer() + m_written;
		}

		std::size_t getWritableSize() const
		{
			return getAllocSize() - m_written;
		}
		std::size_t getAllocSize() const
		{
			return m_buf->getSize();
		}

		void consume(std::size_t size)
		{
			assert(m_read + size <= m_written);	// 읽기 위치 체크
			m_read += size;
		}

		void commit(std::size_t size)
		{
			assert(m_written + size <= getAllocSize());	// 쓰기 위치 체크
			m_written += size;
		}
		// 데이터 미리보기
		void peek(void* data, std::size_t size) const
		{
			assert(m_read + size <= m_written);
			::memcpy(data, getReadableBuffer(), size);
		}

		void pop(std::size_t size)
		{
			consume(size);
		}

		void reset()
		{
			m_read = 0;
			m_written = 0;
		}
	private:
		std::size_t m_read;
		std::size_t m_written;
		StreamBufPtr m_buf;

		friend class OStreamVerifier;
		friend class IStreamVerifier;
	};

	// 기본형들 읽기 쓰기
	template<typename T, typename = typename std::enable_if<std::is_scalar<T>::value>::type>
	Stream& operator>>(Stream& stream, T& value)
	{
		stream.read((void*)&value, sizeof(value));
		return stream;
	}

	template<typename T, typename = typename std::enable_if<std::is_scalar<T>::value>::type>
	Stream& operator<<(Stream& stream, const T& value)
	{
		stream.write(&value, sizeof(value));
		return stream;
	}

	// 문자열 처리
	inline Stream& operator>>(Stream& stream, std::string& str)
	{
		uint32_t len;
		stream >> len;
		if (stream.getReadableSize() < len)
		{
			throw StreamException();
		}

		str.assign((const char*)stream.getReadableBuffer(), len);
		stream.consume(len);
		return stream;
	}

	inline Stream& operator<<(Stream& stream, const std::string& str)
	{
		const uint32_t len = static_cast<uint32_t>(str.length());
		stream << len;
		stream.write(str.c_str(), len);
		return stream;
	}

	// 사용자 정의 타입 처리
	inline Stream& operator>>(Stream& stream, Serializable& serializable)
	{
		serializable.deserialize(stream);
		return stream;
	}

	inline Stream& operator<<(Stream& stream, const Serializable& serializable)
	{
		serializable.serialize(stream);
		return stream;
	}

	// 시퀀스 컨테이너 처리
	template<typename SeqContainerT>
	Stream& readSeqContainer(Stream& stream, SeqContainerT& values)
	{
		values.clear();
		uint32_t size = 0;
		stream >> size;
		values.resize(std::min<size_t>(size, stream.getReadableSize()));	// 검증후 할당
		for (auto& value : values)
		{
			stream >> value;
		}
		return stream;
	}

	template<typename SeqContainerT>
	Stream& writeSeqContainer(Stream& stream, const SeqContainerT& values)
	{
		stream << (uint32_t)values.size();
		for (auto& value : values)
		{
			stream << value;
		}
		return stream;
	}


	// STL 컨테이너 처리
	template<typename T>
	Stream& operator>>(Stream& stream, std::vector<T>& values)
	{
		return readSeqContainer(stream, values);
	}

	template<typename T>
	Stream& operator<<(Stream& stream, const std::vector<T>& values)
	{
		return writeSeqContainer(stream, values);
	}

	template<typename T>
	Stream& operator>>(Stream& stream, std::deque<T>& values)
	{
		return readSeqContainer(stream, values);
	}

	template<typename T>
	Stream& operator<<(Stream& stream, const std::deque<T>& values)
	{
		return writeSeqContainer(stream, values);
	}

	template<typename Key, typename Value>
	Stream& operator>>(Stream& stream, std::unordered_map<Key, Value>& values)
	{
		values.clear();
		uint32_t size = 0;
		stream >> size;
		for (uint32_t i = 0; i < size; ++i)
		{
			Key key;
			Value value;
			stream >> key >> value;
			values[key] = value;
		}
		return stream;
	}

	template<typename Key, typename Value>
	Stream& operator<<(Stream& stream, const std::unordered_map<Key, Value>& values)
	{
		stream << (uint32_t)values.size();
		for (const auto& pair : values)
		{
			stream << pair.first << pair.second;
		}
		return stream;
	}
}