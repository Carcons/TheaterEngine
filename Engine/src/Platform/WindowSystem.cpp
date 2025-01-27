#if !defined(THT_EXCLUDE_WINSYS)
#include <Theater/Engine/Platform/WindowSystem.hpp>

namespace Theater
{
	WindowSystem::WindowSystem()
	{
		/*
			Notice: Events of same size are initialized only once
		*/

		constexpr size_t Resize_PoolChunks = 2ull;
		constexpr size_t Move_PoolChunks = 2ull;
		constexpr size_t WinDpi_PoolChunks= 2ull;

		m_EventsPools.Init<WinResizeEvent>(Resize_PoolChunks, true);
		m_EventsPools.Init<WinMoveEvent>(Move_PoolChunks, true);
	}
}
#endif