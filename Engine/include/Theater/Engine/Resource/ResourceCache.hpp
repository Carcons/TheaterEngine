#pragma once

#include <memory>
#include <unordered_set>
#include <string_view>

namespace Theater
{
	template<typename T>
	class ResourceCache
	{
	public:

		ResourceCache() = default;
		~ResourceCache() = default;

	public:

		void Reserve(size_t n)
		{
			m_Resources.reserve(n);
		}

		std::shared_ptr<T> TryAdd(std::string_view resID)
		{
			m_Resources.insert(T);
		}


		void Remove(std::string_view resID)
		{

		}

	private:
		std::unordered_set<T> m_Resources;
	};
}