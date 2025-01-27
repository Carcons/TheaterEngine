#pragma once

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Theater/Engine/Platform/ILibrary.hpp>

namespace Theater
{
	class Win32Library : public ILibrary
	{
	public:

		Win32Library() = default;
		~Win32Library();

	public:

		bool Load(std::string_view path) override;
		void Free() override;

		void* GetLibNativePtr() const { return m_pLib; }

	public:
		bool IsLoaded() const;
		void* LoadFunction(std::string_view fn) override;

	private:

		HMODULE m_pLib = nullptr;
	};
}


#endif //_WIN32