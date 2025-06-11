#pragma once

#include <assert.h>

#pragma warning(disable : 4661) // Disable warning C4661: "no suitable default constructor exists for class" for template classes

namespace dk	{

	template <typename T>
	class Singleton : private noncopyable
	{
	protected:
		static T* singleton;

	public:
		Singleton()
		{
			assert(singleton == nullptr);
			singleton = static_cast<T*>(this);			
		}
		~Singleton()
		{
			assert(singleton != nullptr);
			singleton = nullptr;
		}
		static T* GetInstancePtr()
		{
			return singleton;
		}
		static T& GetInstance()
		{
			assert(singleton != nullptr);
			return *singleton;
		}
	};
}

#define DKSINGLETON(type) template<> type* dk::Singleton<type>::singleton = nullptr;