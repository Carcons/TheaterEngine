#if defined(_WIN32)

#include <sstream>

#include "Win32Process.hpp"

namespace Theater
{
    IProcess* NewProcess(std::string_view command)
    {
        return new Win32Process(command);
    }

    void DeleteProcess(IProcess* ptr)
    {
        delete ptr;
    }
}

namespace Theater
{
	Win32Process::Win32Process(std::string_view command) : Win32Process()
	{
        if (!command.empty())
            SetCommand(command);
	}

	bool Win32Process::Start()
	{
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        HANDLE hRead, hWrite;
        if (!CreatePipe(&hRead, &hWrite, &saAttr, 0))
            return false;

        if (!SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0))
            return false;

        // Set up the process startup info
        STARTUPINFOA si = { 0 };
        si.cb = sizeof(STARTUPINFOA);
        si.hStdError = hWrite;
        si.hStdOutput = hWrite;
        si.dwFlags |= STARTF_USESTDHANDLES;

        PROCESS_INFORMATION pi = { 0 };

        // Create the process
        if (!CreateProcessA(
            NULL,
            const_cast<char*>(m_Command.c_str()), // Command line
            NULL, // Process security attributes
            NULL, // Thread security attributes
            TRUE, // Inherit handles
            0,    // Creation flags
            NULL, // Environment
            NULL, // Current directory
            &si,  // Startup info
            &pi   // Process information
        )) 
        {
            CloseHandle(hWrite);
            CloseHandle(hRead);
            return false;
        }

        CloseHandle(hWrite);

        // Read the output
        {
            char buffer[4096];
            DWORD bytesRead;
            std::ostringstream oss;
            while (ReadFile(hRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0'; 
                oss << buffer;
            }

            m_Output = oss.str();
        }

        // Clean up
        CloseHandle(hRead);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return true;
	}

    void Win32Process::SetCommand(std::string_view command)
    {
        m_Command = std::string(command);
    }

    std::string_view Win32Process::GetOutput()
    {
        return m_Output;
    }
}

#endif //_WIN32