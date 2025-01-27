#if defined(_WIN32)

#include <cassert>

#include "Win32Library.hpp"

namespace Theater
{
	ILibrary* NewLibrary()
	{
		return new Win32Library;
	}

	void DeleteLibrary(ILibrary* ptr)
	{
		delete ptr;
	}
}

namespace Theater
{
	Win32Library::~Win32Library()
	{
		Free();
	}

	bool Win32Library::Load(std::string_view path)
	{
		Free();

		m_pLib = LoadLibrary(path.data());

		if (m_pLib)
			return true;

		return false;
	}

	void Win32Library::Free()
	{
		if (m_pLib)
		{
			::FreeLibrary(m_pLib);
			m_pLib = nullptr;
		}
	}

	bool Win32Library::IsLoaded() const
	{
		return m_pLib != nullptr;
	}

	void* Win32Library::LoadFunction(std::string_view fn)
	{
		assert(m_pLib && "Library not loaded");

		return ::GetProcAddress(reinterpret_cast<HMODULE>(m_pLib), fn.data());
	}
}

#endif //_WIN32