#pragma once

#include "Vector4.hpp"

#if defined(THT_SSE) || defined(THT_SVE)

#if defined(THT_SSE)

#include <emmintrin.h>

#else 
//#include..sve

#endif // THT_SSE

#endif


#pragma region SIMD Intrinsics

#if defined(THT_SSE) || defined(THT_SVE)

#if defined(THT_SSE)

#define VEC __m128
#define ADD(vec1, vec2) _mm_add_ps(vec1, vec2)
#define SUB(vec1, vec2) _mm_sub_ps(vec1, vec2)
#define MUL(vec1, vec2) _mm_mul_ps(vec1, vec2)
#define DIV(vec1, vec2) _mm_div_ps(vec1, vec2)
#define SET(vec) _mm_setr_ps(vec.x, vec.y, vec.z, vec.w)
#define STORE(fVec, vec) _mm_store_ps(fVec, vec)
#define SQRT(vec) _mm_sqrt_ps(vec)
#define RSQRT(vec) _mm_rsqrt_ps(vec)

#elif defined(THT_SVE)


#endif 

namespace Theater
{
	struct VECTOR final
	{
		VEC _vec;

		explicit VECTOR(const Vector4& vec) : _vec(SET(vec))
		{}

		explicit VECTOR(const Vector3& vec) : _vec(SET(Vector4(vec)))
		{}

		explicit VECTOR(__m128 vec) : _vec(vec)
		{}

		Vector4 GetVec4() const
		{
			ALIGN16 float fVec[4];
			STORE(fVec, _vec);

			return Vector4(fVec);
		}

		VECTOR& operator += (const VECTOR& vec);
		VECTOR& operator -= (const VECTOR& vec);
		VECTOR& operator *= (const VECTOR& vec);
		VECTOR& operator /= (const VECTOR& vec);
	};

	inline VECTOR& VECTOR::operator += (const VECTOR& vec)
	{
		_vec = ADD(_vec, vec._vec);
		return *this;
	}

	inline VECTOR& VECTOR::operator -= (const VECTOR& vec)
	{
		_vec = SUB(_vec, vec._vec);
		return *this;
	}

	inline VECTOR& VECTOR::operator *= (const VECTOR& vec)
	{
		_vec = MUL(_vec, vec._vec);
		return *this;
	}

	inline VECTOR& VECTOR::operator /= (const VECTOR& vec)
	{
		_vec = DIV(_vec, vec._vec);
		return *this;
	}

	inline VECTOR operator + (const VECTOR& lhs, const VECTOR rhs)
	{
		return VECTOR(ADD(lhs._vec, rhs._vec));
	}

	inline VECTOR operator - (const VECTOR& lhs, const VECTOR& rhs)
	{
		return VECTOR(SUB(lhs._vec, rhs._vec));
	}

	inline VECTOR operator * (const VECTOR& lhs, const VECTOR& rhs)
	{
		return VECTOR(MUL(lhs._vec, rhs._vec));
	}

	inline VECTOR operator / (const VECTOR& lhs, const VECTOR& rhs)
	{
		return VECTOR(DIV(lhs._vec, rhs._vec));
	}
}
#endif // THT_SSE || THT_SVE


#pragma endregion