#pragma once

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Theater/Engine/Utility/String.hpp>
#include <Theater/Engine/Platform/IProcess.hpp>

namespace Theater
{
	class Win32Process : public IProcess
	{
	public:

		Win32Process() = default;
		explicit Win32Process(std::string_view command);
		~Win32Process() = default;

		Win32Process(const Win32Process&) = delete;
		Win32Process& operator = (const Win32Process&) = delete;

	public:

		bool Start() override;
		void SetCommand(std::string_view command) override;

	public:

		std::string_view GetOutput() override;

	private:

		std::string m_Command;
		std::string m_Output;
	};
}
#endif