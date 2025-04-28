#pragma once

#include <memory>
#include <atomic>

#define CLASS_SHARED_PTR(T) \
	class T;	\
	using T##Ptr = std::shared_ptr<T>;	\

#define STRUCT_SHARED_PTR(T) \
	struct T;	\
	using T##Ptr = std::shared_ptr<T>;	\

#ifndef _MSC_VER
#define UNREFERENCED_PARAMETER(P) (P)
#define IN
#define OUT
#endif // !_MSC_VER


namespace dk
{
	class noncopyable
	{
	protected:
		noncopyable() = default;
		~noncopyable() = default;
	private:
		noncopyable(const noncopyable&) = delete;
		noncopyable& operator=(const noncopyable&) = delete;
	};

	using Number = int32_t;
	using AtomicNumber = std::atomic<Number>;
	CLASS_SHARED_PTR(StreamBuf);
}