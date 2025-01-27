#pragma once

#if defined(_WIN32)

#include <Windows.h>

#endif // _WIN32

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <iostream>

#include <Theater/Engine/Platform/WindowSystem.hpp>
#include <Theater/Engine/Platform/ILibrary.hpp>
#include <Theater/Engine/Graphics/RenderingSystem.hpp>
#include <Theater/Engine/Game/Game.hpp>

#include "TheaterGameplay.hpp"

using namespace Theater;

namespace
{
    Theater::CommandLineArgs GetCmdArgs();
}

void Theater::TheaterMain(const CommandLineArgs& args)
{
    std::function<void* ()> GameLoadFn;
    std::function<void(void*)> GameFreeFn;

    ILibrary* const pGameLibrary = NewLibrary();

    // Loading Gamelibrary
    {
        if (!pGameLibrary->Load(Detail::GAME_LIBRARY))
        {
            throw std::runtime_error("Can't found Theater Game library");
        }

        using namespace Detail;
        using namespace Detail::GameplayFunctions;

        GameLoadFn = THT_LoadLibFn(pGameLibrary, void* (), THEATER_NEWGAME);
        GameFreeFn = THT_LoadLibFn(pGameLibrary, void(void*), THEATER_FREEGAME);
    }
    Game* const pGame = reinterpret_cast<Game*>(GameLoadFn());
    pGame->m_CmdArgs = args;

    WindowSystem* const pWinSys = NewWindowSystem();
    {
		constexpr uint32_t DefaultWidth = 1920u;
		constexpr uint32_t DefaultHeight = 1080u;

		uint32_t w = DefaultWidth, h = DefaultHeight;
		const auto Monitors = pWinSys->GetMonitors();
		for (auto& monitor : Monitors)
		{
			if (monitor.IsPrimary)
			{
				w = monitor.Width;
				h = monitor.Height;

				break;
			}
		}

		WindowSpecs winSpecs{};
		winSpecs.Width = w;
		winSpecs.Height = h;
		winSpecs.Title = "TheaterGame";

		pWinSys->ConstructWindow(winSpecs);
    }
    RenderingSystem* const pRenderingSys = new RenderingSystem;

    pGame->OnGameReady_External();

    auto lastTime = std::chrono::high_resolution_clock::now();
    float deltaTime = 0.f;
    while (pWinSys->IsOpen())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        pWinSys->ProcessEvents();

        pGame->OnUpdate(deltaTime.count());
        pGame->OnDraw();
        
        pWinSys->SwapBuffers();
    }

    delete pRenderingSys;
    DeleteWindowSystem(pWinSys);
    GameFreeFn(pGame);
    DeleteLibrary(pGameLibrary);
}

#pragma region PlatformEntryPoint
#if defined(_WIN32)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    #if defined(_DEBUG) || defined(DEBUG)
    AllocConsole();

    SetConsoleTitle("TheaterEngine Debug Window");

    FILE* fp = nullptr;

    freopen_s(&fp, "CONOUT$", "w", stdout);  
    freopen_s(&fp, "CONOUT$", "w", stderr);

    std::ios::sync_with_stdio(true);
    #endif

    TheaterMain(GetCmdArgs());
    return 0;
}

namespace
{
    Theater::CommandLineArgs GetCmdArgs()
    {
        Theater::CommandLineArgs args;
        const LPWSTR pCommandLine = ::GetCommandLineW();

        int32_t argc = 0;
        LPWSTR* pArgv = ::CommandLineToArgvW(pCommandLine, &argc);
        if (!pArgv)
            return args;

        args.reserve(argc);

        for (uint32_t i = 0; i < argc; i++) 
        {
            const int32_t SizeNeeded = ::WideCharToMultiByte(CP_UTF8, 0, pArgv[i], -1, nullptr, 0, nullptr, nullptr);
            std::string arg(SizeNeeded - 1, '\0');

            ::WideCharToMultiByte(CP_UTF8, 0, pArgv[i], -1, arg.data(), SizeNeeded, nullptr, nullptr);
            args.push_back(std::move(arg));
        }

        ::LocalFree(pArgv);

        return args;
    }
}

#else // MAC OS, LINUX

namespace
{
    Theater::CommandLineArgs GetCmdArgs()
    {
        std::vector<const std::string> args;
        return args;
    }
}

#endif
#pragma endregion