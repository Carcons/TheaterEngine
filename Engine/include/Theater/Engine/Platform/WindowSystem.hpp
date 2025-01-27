#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <functional>

#include "../EngineAPI.hpp"
#include "../System.hpp"
#include "../Event/EventDispatcher.hpp"
#include "../Utility/MemoryPool.hpp"

#include "WinSysEvents.hpp"

namespace Theater
{
	enum class WindowState : uint32_t
	{
		Normal,
		Minimized,
		Maximized,
		Fullscreen
	};

	struct WindowSpecs
	{
		WindowState State;
		uint32_t Width;
		uint32_t Height;
		uint32_t x;
		uint32_t y;
		std::string Title;
	};

	struct MonitorInfo
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;
		bool IsPrimary;
	};

	#if defined (_WIN32)
	using Win32WndProc = std::function<bool(void* hWnd, uint32_t msg, void* wParam, void* lParam)>;
	#endif

	class ENGINE_API WindowSystem : public System<WindowSystem>, public EventDispatcher
	{
	public:

		WindowSystem();
		virtual ~WindowSystem() = default;

	public:

		#if defined(_WIN32)
		virtual void InstallWin32WndProc(Win32WndProc&& handler) = 0;
		#endif

		virtual void SetAppInstancePtr(void* pAppInstance) = 0;
		virtual void SwapBuffers() = 0;
		virtual void SetDPIAware() = 0;
		virtual void ProcessEvents() = 0;
		virtual void SetVSync(bool enable)= 0;
		virtual void SetTitle(std::string_view title) = 0;
		virtual void SetState(WindowState state) = 0;

		virtual void ConstructWindow(const WindowSpecs& specs) = 0;

	public:

		virtual float GetContentScale() const = 0;
		virtual std::vector<MonitorInfo> GetMonitors() const = 0;
		virtual void* GetNativeWinPtr() const = 0;
		virtual WindowState GetWindowState() const = 0;

		bool IsVSyncEnabled() const { return m_VSync; }
		bool IsOpen() const { return m_Open; }
		std::string_view GetTitle() const { return m_Title; }

	protected:

		virtual void CreateRenderingCtx() = 0;
		virtual void DestroyRenderingCtx() = 0;
		virtual void MakeRenderingCtxCurrent() = 0;

	protected:

		bool m_VSync = false;
		bool m_Open = false;

		uint32_t m_Width = 0u;
		uint32_t m_Height = 0u;
		uint32_t m_X = 0u;
		uint32_t m_Y = 0u;

		std::string m_Title;

		MultiMemPool m_EventsPools;
	};

	ENGINE_API [[nodiscard]] WindowSystem* NewWindowSystem();
	ENGINE_API void DeleteWindowSystem(WindowSystem* ptr);
}
