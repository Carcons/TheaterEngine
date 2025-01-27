#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif // _WIN32

#if defined(WITH_EDITOR)

#include "TheaterEditor/Editor.hpp"

#endif // WITH_EDITOR


#include <Theater/Engine/Platform/WindowSystem.hpp>
#include <Theater/Engine/Graphics/Renderer.hpp>

using namespace Theater;

namespace
{
	WindowSystem* pWinSys = nullptr;
	Renderer* pRenderer = nullptr;
}

int TheaterMain(int argc, char** argv)
{
	pWinSys = NewWindowSystem();
	pWinSys->SetDPIAware();

	assert(pWinSys && "Can't create WindowSystem");
	if (!pWinSys)
		return -1;

	// Window
	{

		WindowSpecs winSpecs{};

		pWinSys->ConstructWindow(winSpecs);
		pWinSys->CreateRenderingCtx({});
		pWinSys->MakeRenderingCtxCurrent();
		pWinSys->SetState(WindowState::Maximized);
	}
	
	// Renderer
	pRenderer = new Renderer;

	assert(pRenderer && "Can't create renderer");
	if (!pRenderer)
		return -2;

	#if defined(WITH_EDITOR)
	Editor::Init();
	#endif
	
	while (pWinSys->IsOpen())
	{
		pWinSys->ProcessEvents();

		pRenderer->ClearBuffers();

		#if defined(WITH_EDITOR)
		Editor::Update();
		Editor::Render();
		#endif

		pWinSys->SwapBuffers();
	}


	#if defined(WITH_EDITOR)
	Editor::Shutdown();
	#endif

	delete pRenderer;
	pRenderer = nullptr;

	DeleteWindowSystem(pWinSys);

	return 0;
}


#if defined(WITH_EDITOR)


int main(int argc, char** argv)
{
	return TheaterMain(argc, argv);
}


#else


#if defined(_WIN32)

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return TheaterMain(0,0);
}

#else // MAC OS, LINUX (future implementations)


#endif



#endif 