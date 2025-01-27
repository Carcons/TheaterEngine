#include <new>
#include <cassert>

#include <Theater/Engine/Utility/MemoryPool.hpp>

namespace Theater
{
	MemPool::MemPool(size_t dataSize, size_t numChunks, bool allowGrow, bool init) :
		m_DataSize(dataSize),
		m_NumChunks(numChunks),
		m_NumBlocks(0u),
		m_AllowGrow(allowGrow),
		m_pHead(nullptr),
		m_ppBlocksArray(nullptr)
	{
		if (init)
			Init();
	}

	MemPool::~MemPool()
	{
		delete m_ppBlocksArray;
		m_pHead = nullptr;
		m_ppBlocksArray = nullptr;
	}

	void MemPool::Init()
	{
		/*
		*			    0							   N-1
		*	Block:	[Header][data][Header][data]....[Header][data]
		*	Bytes:		C      X      C      X          C      X
		*
		*	Where C is CHUNK_HEADER_SIZE, X is m_DataSize, in bytes
		*/
		GrowBlocksArray();
	}

	bool MemPool::GrowBlocksArray()
	{
		const size_t AllocSize = sizeof(Byte*) * (m_NumBlocks + 1);
		Byte** const ppNewBlocksArray = static_cast<Byte**>(::operator new(AllocSize));

		if (!ppNewBlocksArray)
			return false;

		for (size_t i = 0ull; i < m_NumBlocks; i++)
			ppNewBlocksArray[i] = m_ppBlocksArray[i];


		#pragma warning( push )
		#pragma warning( disable : 6386)
		ppNewBlocksArray[m_NumBlocks] = AllocateNewBlock();
		#pragma warning( pop ) 

		if (m_pHead)
		{
			Byte* pCurr = m_pHead;
			Byte* pNext = GetNext(m_pHead);
			while (pNext)
			{
				pCurr = pNext;
				pNext = GetNext(pNext);
			}
			SetNext(pCurr, ppNewBlocksArray[m_NumBlocks]);
		}
		else
		{
			#pragma warning( push )
			#pragma warning( disable : 6385)
			m_pHead = ppNewBlocksArray[m_NumBlocks];
			#pragma warning( pop ) 
		}

		if (m_ppBlocksArray)
			::operator delete(m_ppBlocksArray);

		m_ppBlocksArray = ppNewBlocksArray;
		++m_NumBlocks;

		return true;
	}

	Byte* MemPool::AllocateNewBlock()
	{
		const size_t ChunkSize = (m_DataSize + CHUNK_HEADER_SIZE);
		const size_t BlockSize = ChunkSize * m_NumChunks;

		Byte* const pNewMemory = static_cast<Byte*>(::operator new(BlockSize));
		if (!pNewMemory)
			return nullptr;

		Byte* const pEnd = pNewMemory + BlockSize;
		Byte* pCurr = pNewMemory;

		while (pCurr < pEnd)
		{
			Byte* const pNext = pCurr + ChunkSize;
			Byte** const ppChunkHeader = reinterpret_cast<Byte**>(pCurr);

			if (pNext < pEnd)
				ppChunkHeader[0] = pNext;
			else
				ppChunkHeader[0] = nullptr;

			pCurr += ChunkSize;
		}

		return pNewMemory;
	}

	void* MemPool::Allocate()
	{
		if (!m_pHead)
		{
			if (!m_AllowGrow)
				return nullptr;

			if (!GrowBlocksArray())
				return nullptr;
		}

		Byte* const pReturn = m_pHead;

		m_pHead = GetNext(m_pHead);

		return pReturn + CHUNK_HEADER_SIZE;
	}

	void MemPool::Deallocate(void* pMemory)
	{
		if (pMemory)
		{
			Byte* const pBlock = static_cast<Byte*>(pMemory) - CHUNK_HEADER_SIZE;
			SetNext(pBlock, m_pHead);

			m_pHead = pBlock;
		}
	}
}