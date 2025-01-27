#if defined(_WIN32)

#include <glad/glad.h>
#include <cassert>

#include "Win32WindowSystem.hpp"

#pragma region GL

namespace Theater
{
	constexpr uint32_t RENDERING_CTX_MAJOR = 4u;
	constexpr uint32_t RENDERING_CTX_MINOR = 1u;
	constexpr uint8_t RENDERING_CTX_COLOR = 32u;
	constexpr uint8_t RENDERING_CTX_DEPTH = 24u;
	constexpr uint8_t RENDERING_CTX_STENCIL = 8u;

	constexpr int32_t GL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
	constexpr int32_t GL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
	constexpr int32_t GL_CONTEXT_FLAGS_ARB = 0x2094;
	constexpr int32_t GL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;
	constexpr int32_t GL_CONTEXT_PROFILE_MASK_ARB = 0x9126;
}

namespace
{
	typedef const char* (WINAPI* PFNGLGETEXTENSIONSSTRINGEXTPROC) (void);
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);
	typedef int(WINAPI* PFNWGLGETSWAPINTERVALEXTPROC)(void);
	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC, HGLRC, const int*);

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	PFNWGLSWAPINTERVALEXTPROC wglSwapInterval = nullptr;
}
#pragma endregion

namespace Theater
{
	WindowSystem* NewWindowSystem()
	{
		return new Win32WindowSystem();
	}

	void DeleteWindowSystem(WindowSystem* ptr)
	{
		delete ptr;
	}
}

namespace Theater
{
	constexpr char* THT_WNDCLASS = "TheaterWndClass";

	Win32WindowSystem::Win32WindowSystem() :
		m_hInstance(GetModuleHandle(nullptr)),
		m_hRC(nullptr),
		m_hWnd(nullptr),
		m_hDC(nullptr)
	{}

	Win32WindowSystem::~Win32WindowSystem()
	{
		DestroyRenderingCtx();

		if (m_hWnd)
		{
			::DestroyWindow(m_hWnd);
			m_hWnd = nullptr;
		}
	}

	void Win32WindowSystem::SetState(WindowState state)
	{
		int32_t showCmd = 0;

		switch (state)
		{
		case WindowState::Normal: { showCmd = SW_SHOWNORMAL; break; }
		case WindowState::Maximized: { showCmd = SW_SHOWMAXIMIZED; break; }
		case WindowState::Minimized: { showCmd = SW_SHOWMINIMIZED; break; }
		case WindowState::Fullscreen: { showCmd = SHOW_FULLSCREEN; break; }
		default: showCmd = SW_SHOWNORMAL;
		}

		::ShowWindow(m_hWnd, showCmd);
		::UpdateWindow(m_hWnd);
	}

	void Win32WindowSystem::SetTitle(std::string_view title)
	{
		m_Title = title;
		SetWindowText(m_hWnd, m_Title.c_str());
	}

	void Win32WindowSystem::CreateRenderingCtx()
	{
		const PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
			PFD_TYPE_RGBA,  
			RENDERING_CTX_COLOR,   
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			RENDERING_CTX_DEPTH,  
			RENDERING_CTX_STENCIL, 
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		const int32_t PixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
		::SetPixelFormat(m_hDC, PixelFormat, &pfd);

		HGLRC tempRC = ::wglCreateContext(m_hDC);
		::wglMakeCurrent(m_hDC, tempRC);

		if (!::wglCreateContextAttribsARB)
			::wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) ::wglGetProcAddress("wglCreateContextAttribsARB");

		if (!::wglSwapInterval)
			::wglSwapInterval = (PFNWGLSWAPINTERVALEXTPROC) ::wglGetProcAddress("wglSwapIntervalEXT");

		const int32_t AttribList[] =
		{
			GL_CONTEXT_MAJOR_VERSION_ARB, RENDERING_CTX_MAJOR,
			GL_CONTEXT_MINOR_VERSION_ARB, RENDERING_CTX_MINOR,
			GL_CONTEXT_FLAGS_ARB, 0,
			GL_CONTEXT_PROFILE_MASK_ARB,
			GL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
		};

		m_hRC = ::wglCreateContextAttribsARB(m_hDC, nullptr, AttribList);
		::wglDeleteContext(tempRC);
	}

	void Win32WindowSystem::MakeRenderingCtxCurrent()
	{
		if (!m_hRC)
			return;

		::wglMakeCurrent(m_hDC, m_hRC);
	}

	void Win32WindowSystem::SwapBuffers()
	{
		if (!m_hDC)
			return;

		::SwapBuffers(m_hDC);
	}

	void Win32WindowSystem::DestroyRenderingCtx()
	{
		if (!m_hRC)
			return;

		::wglMakeCurrent(NULL, NULL);
		::wglDeleteContext(m_hRC);
		::ReleaseDC(m_hWnd, m_hDC);
	}

	void Win32WindowSystem::SetAppInstancePtr(void* pAppInstance)
	{
		m_hInstance = reinterpret_cast<HINSTANCE>(pAppInstance);
	}

	void Win32WindowSystem::InstallWin32WndProc(Win32WndProc&& handler)
	{
		m_WndProcFn = std::forward<Win32WndProc>(handler);
	}

	namespace { BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData); }
	std::vector<MonitorInfo> Win32WindowSystem::GetMonitors() const
	{
		std::vector<MonitorInfo> monitors;
		::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors));

		return monitors;
	}

	void Win32WindowSystem::SetDPIAware()
	{
		::SetProcessDPIAware();
	}

	WindowState Win32WindowSystem::GetWindowState() const
	{
		WINDOWPLACEMENT windowPlacement;
		WindowState state{};
		windowPlacement.length = sizeof(WINDOWPLACEMENT);

		::GetWindowPlacement(m_hWnd, &windowPlacement);

		switch (windowPlacement.showCmd)
		{
		case SW_SHOWMAXIMIZED:
			state = WindowState::Maximized;
			break;
		case SW_SHOWMINIMIZED:
			state = WindowState::Minimized;
			break;
		case SW_SHOWNORMAL:
			state = WindowState::Normal;
			break;
		};
		
		return state;
	}

	void Win32WindowSystem::ConstructWindow(const WindowSpecs& specs)
	{
		WNDCLASS wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = Win32WindowSystem::WndProc;
		wc.hInstance = m_hInstance;
		wc.lpszClassName = THT_WNDCLASS;
		wc.hbrBackground = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		RegisterClass(&wc);

		const int32_t style = WS_OVERLAPPEDWINDOW | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		int32_t exStyle = 0;

		RECT windowRect = { (int32_t)specs.x, (int32_t)specs.y, (int32_t)specs.Width, (int32_t)specs.Height };
		::AdjustWindowRect(&windowRect, style, FALSE);

		m_hWnd = CreateWindowEx(
			exStyle,
			THT_WNDCLASS, // WndClass
			specs.Title.c_str(), // Title
			style, // Style
			specs.x, // x
			specs.y, // y
			specs.Width, // w
			specs.Height, // h
			nullptr, // parent
			nullptr, // menu
			m_hInstance, // winapp pInstance
			this // extra data
		);

		assert(m_hWnd && "Failed to create window, m_hWnd is nullptr");

		m_hDC = ::GetDC(m_hWnd);
		::UpdateWindow(m_hWnd);

		SetState(specs.State);

		m_X = specs.x;
		m_Y = specs.y;
		m_Title = specs.Title;
		m_Width = specs.Width;
		m_Height = specs.Height;

		m_Open = true;

		CreateRenderingCtx();
		MakeRenderingCtxCurrent();
	}

	float Win32WindowSystem::GetContentScale() const
	{
		const UINT DPI = ::GetDpiForWindow(m_hWnd);
		return DPI / USER_DEFAULT_SCREEN_DPI;
	}

	void Win32WindowSystem::ProcessEvents()
	{
		if (HasPendingEvents())
			DispatchEvents();

		MSG msg{};

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Win32WindowSystem::SetVSync(bool enable)
	{
		if (!::wglSwapInterval)
			return;

		m_VSync = enable;
		::wglSwapInterval(enable);
	}

	LRESULT CALLBACK Win32WindowSystem::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Win32WindowSystem* const pWindow = reinterpret_cast<Win32WindowSystem*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		static bool sEnterResizing = false;
		static bool sEnterMoving = false;
		static uint32_t sResizingWidth = 0u;
		static uint32_t sResizingHeight = 0u;
		static uint32_t sMoveX = 0u;
		static uint32_t sMoveY = 0u;


		if (pWindow && pWindow->m_WndProcFn)
		{
			if (pWindow->m_WndProcFn((void*)hWnd, (uint32_t)msg, (void*)wParam, (void*)lParam))
				return true;
		}

		switch (msg)
		{
		case WM_CREATE:
		{
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			::SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));

			return true;
		}
		//case WM_QUIT:
		//{
		//	::DestroyWindow(hWnd);
		//	return true;
		//}
		case WM_SIZE:
		{
			sEnterResizing = true;
			sResizingWidth = LOWORD(lParam);
			sResizingHeight= HIWORD(lParam);

			return true;
		} 
		case WM_MOVE:
		{
			sEnterMoving = true;
			sMoveX = LOWORD(lParam);
			sMoveY = HIWORD(lParam);

			return true;
		}
		case WM_EXITSIZEMOVE:
		{
			if ((sEnterResizing || sEnterMoving) && pWindow && pWindow->HasEventListeners())
			{
				if (sEnterResizing)
				{
					MemPool* const pEventPool = pWindow->m_EventsPools.Get<WinResizeEvent>();

					pWindow->PublishEvent(MemPoolSharedPtr<WinResizeEvent>(pEventPool, WinResizeEvent(sResizingWidth, sResizingHeight)));

					sResizingWidth = 0u;
					sResizingHeight = 0u;
					sEnterResizing = false;
				}
				
				if (sEnterMoving)
				{
					MemPool* const pEventPool = pWindow->m_EventsPools.Get<WinMoveEvent>();

					pWindow->PublishEvent(MemPoolSharedPtr<WinMoveEvent>(pEventPool, WinMoveEvent(sMoveX, sMoveY)));

					sMoveX = 0u;
					sMoveY = 0u;
					sEnterMoving = false;
				}
			}

			break;
		}
		case WM_SYSCOMMAND:
		{
			if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return true;

			break;
		}
		case WM_DESTROY:
		{
			if (pWindow)
			{
				pWindow->m_Open = false;
				::ReleaseDC(pWindow->m_hWnd, pWindow->m_hDC);
				UnregisterClass(THT_WNDCLASS, pWindow->m_hInstance);
				pWindow->m_hWnd = nullptr;
			}
			return true;
		}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

#pragma region Win32 Helpers

namespace Theater
{
	namespace
	{
		BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
		{
			std::vector<MonitorInfo>* const pMonitors = reinterpret_cast<std::vector<MonitorInfo>*>(dwData);

			MONITORINFOEX monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFOEX);

			if (GetMonitorInfo(hMonitor, &monitorInfo))
			{
				MonitorInfo info;
				info.Name = monitorInfo.szDevice;
				info.Width = monitorInfo.rcMonitor.left + monitorInfo.rcMonitor.right;
				info.Height = monitorInfo.rcMonitor.bottom + monitorInfo.rcMonitor.top;
				info.IsPrimary = (monitorInfo.dwFlags & MONITORINFOF_PRIMARY) != 0;

				pMonitors->push_back(info);
			}

			return TRUE;
		}
	}
}


#pragma endregion

#endif //_WIN32