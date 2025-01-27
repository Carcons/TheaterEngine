#pragma once

#pragma region SIMD_BLOCK
#if defined(THT_SSE) || defined(THT_SVE)

#include <cstdlib> //aligned_malloc

#define ALIGN16 alignas(16)
#define ALIGNED_MALLOC_OP(Type) \
	static void* Type::operator new(u64 size) \
	{ \
		return _aligned_malloc(size, alignof(Type)); \
	} \
	static void Type::operator delete(void* ptr) \
	{	\
		_aligned_free(ptr);\
	}

#else

#define ALIGN16
#define ALIGNED_MALLOC_OP(Type)

#endif // THT_SIMD


namespace Theater
{
	constexpr inline bool IsUsingSSE()
	{
#if defined(THT_SSE)
		return true;
#else
		return false;
#endif
	}

	constexpr inline bool IsUsingSVE()
	{
#if defined(THT_SVE)
		return true;
#else 
		return false;
#endif
	}
}



#pragma endregion