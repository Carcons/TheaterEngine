#pragma once

#include <cassert>

namespace Theater
{
	template<class T>
	class System
	{
	public:

		System()
		{
			assert(!s_pInstance);
			s_pInstance = static_cast<T*>(this);
		}

		~System()
		{
			assert(s_pInstance);
			s_pInstance = nullptr;
		}

		System(const System<T>&) = delete;
		System& operator = (const System<T>&) = delete;

	public:

		static T& Get()
		{
			assert(s_pInstance);
			return *s_pInstance;
		}

		static T* GetPtr()
		{
			return s_pInstance;
		}

	private:

		inline static T* s_pInstance = nullptr;
	};
}

