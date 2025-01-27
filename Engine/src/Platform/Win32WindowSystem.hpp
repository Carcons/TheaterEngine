#pragma once

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Theater/Engine/Platform/WindowSystem.hpp>

namespace Theater
{
	class Win32WindowSystem : public WindowSystem
	{
	public:

		Win32WindowSystem();
		~Win32WindowSystem();

	public:
		
		void InstallWin32WndProc(Win32WndProc&& handler);
		void SetAppInstancePtr(void* pAppInstance) override;
		void SetDPIAware() override;

		void SwapBuffers() override;
		void SetState(WindowState state) override;
		void SetTitle(std::string_view title) override;
		void SetVSync(bool enable) override;
		void ProcessEvents() override;

		void ConstructWindow(const WindowSpecs& specs) override;


	public:

		float GetContentScale() const override;
		void* GetNativeWinPtr() const { return m_hWnd; }
		bool IsOpen() const { return m_Open; }
		std::string_view GetTitle() const { return m_Title; }
		WindowState GetWindowState() const override;

		std::vector<MonitorInfo> GetMonitors() const override;

	private:

		void CreateRenderingCtx() override;
		void DestroyRenderingCtx() override;
		void MakeRenderingCtxCurrent() override;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:

		HINSTANCE m_hInstance;
		Win32WndProc m_WndProcFn;
		HGLRC m_hRC;
		HDC m_hDC;
		HWND m_hWnd;

	};
}
#endif //_WIN32