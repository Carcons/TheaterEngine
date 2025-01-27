#pragma once

#include <memory>
#include <unordered_map>
#include <cstdint>

#include "../EngineAPI.hpp"

#pragma region MemPool
namespace Theater
{
	using Byte = unsigned char;
	class ENGINE_API MemPool final
	{
	public:

		MemPool(size_t dataSize = 0ull, size_t numChunks = 0ull, bool allowGrow = false, bool init = false);

		MemPool(const MemPool&) = delete;
		MemPool& operator = (const MemPool&) = delete;

		MemPool(MemPool&& other) noexcept
		{
			*this = std::forward<MemPool>(other);
		}

		MemPool& operator = (MemPool&& other) noexcept
		{
			if (this != &other)
			{
				m_NumChunks = other.m_NumChunks;
				m_DataSize = other.m_DataSize;
				m_AllowGrow = other.m_AllowGrow;
				m_NumBlocks = other.m_NumBlocks;
				m_pHead = other.m_pHead;
				m_ppBlocksArray = other.m_ppBlocksArray;

				other.m_AllowGrow = false;
				other.m_DataSize = 0ull;
				other.m_NumBlocks = 0ull;
				other.m_pHead = nullptr;
				other.m_ppBlocksArray = nullptr;
			}

			return *this;
		}

		~MemPool();

	public:

		void Init();
		void* Allocate();
		void Deallocate(void* pMemory);

	private:

		Byte* AllocateNewBlock();
		bool GrowBlocksArray();

		void SetNext(Byte* pBlockToChange, Byte* pNewNext) { *reinterpret_cast<Byte**>(pBlockToChange) = pNewNext; }
		Byte* GetNext(Byte* pBlock) { return *reinterpret_cast<Byte**>(pBlock); }

	private:

		Byte** m_ppBlocksArray;
		Byte* m_pHead;

		size_t m_NumBlocks;
		size_t m_DataSize;
		size_t m_NumChunks;

		bool m_AllowGrow;

	private:
		constexpr static inline size_t CHUNK_HEADER_SIZE = sizeof(Byte*);
	};
}
#pragma endregion

#pragma region MultiMemPool
namespace Theater
{
	class MultiMemPool
	{
	public:

		MultiMemPool() = default;

		MultiMemPool(const MultiMemPool&) = delete;
		MultiMemPool& operator = (const MultiMemPool&) = delete;

		MultiMemPool(MultiMemPool&& other) noexcept
		{
			*this = std::forward<MultiMemPool>(other);
		}

		MultiMemPool& operator = (MultiMemPool&& other) noexcept
		{
			if (this != &other)
			{
				m_Pools = std::move(other.m_Pools);
			}

			return *this;
		}

	public:

		template<typename T>
		bool Init(size_t numChunks, bool allowGrow)
		{
			if (Get<T>())
				return false;

			auto it = m_Pools.emplace(sizeof(T), std::make_unique<MemPool>(sizeof(T), numChunks, allowGrow, true));
			return it.second;
		}

		template<typename TFirst, typename... TRest>
		bool MultipleInit(size_t numChunks, bool allowGrow)
		{
			const bool Result = Init<TFirst>(numChunks, allowGrow) && (Init<TRest>(numChunks, allowGrow) && ...);
			return Result;
		}

		template<typename T>
		bool Insert(MemPool&& pool)
		{
			assert(Get<T>() && "Pool for sizeof(T) already inserted");

			auto it = m_Pools.insert({ sizeof(T), std::make_unique<MemPool>(std::forward<MemPool>(pool)) });
			return it.second;
		}

		template<typename T>
		bool Remove()
		{
			auto it = m_Pools.find(sizeof(T));
			if (it != m_Pools.end())
			{
				m_Pools.erase(sizeof(T));
				return true;
			}

			return false;
		}

		template<typename T>
		MemPool* Get()
		{
			auto it = m_Pools.find(sizeof(T));
			if (it != m_Pools.end())
				return it->second.get();

			return nullptr;
		}

	private:

		std::unordered_map<size_t, std::unique_ptr<MemPool>> m_Pools;
	};
}
#pragma endregion

#pragma region Helpers
namespace Theater
{
	template<typename T>
	[[nodiscard]] inline T* MemPoolNewPtr(MemPool* pPool, T&& obj)
	{
		void* const pMem = pPool->Allocate();
		if (!pMem)
			return nullptr;

		T* const pObj = new (pMem) T(std::forward<T>(obj));
		return pObj;
	}

	template<typename T>
	inline void MemPoolDeletePtr(MemPool* pPool, T* ptr)
	{
		ptr->~T();
		pPool->Deallocate(ptr);
	}

	template<typename T>
	[[nodiscard]] inline std::shared_ptr<T> MemPoolSharedPtr(MemPool* pPool, T&& obj)
	{
		T* const pObj = MemPoolNewPtr<T>(pPool, std::forward<T>(obj));

		if (!pObj)
			return nullptr;

		return std::shared_ptr<T>(
			pObj,
			[pPool](T* pObj)
			{
				MemPoolDeletePtr<T>(pPool, pObj);
			}
		);
	}

	//template<typename T>
	//[[nodiscard]] inline auto MemPoolUniquePtr(MemPool* pPool, T&& obj)
	//{
	//	T* const pObj = MemPoolNewPtr<T>(pPool, std::forward<T>(obj));

	//	auto deleter = [pPool](auto* pObj) { MemPoolDeletePtr<T>(pPool, pObj); };

	//	if (!pObj)
	//		return std::unique_ptr<T, decltype(deleter)>(nullptr);

	//	return std::unique_ptr<T, decltype(deleter)>(pObj);
	//}

}
#pragma endregion