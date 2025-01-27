#include <Theater/GameplayAPI/GameplayAPI.hpp>

extern "C"
{
	GAMEPLAY_API int Theater_InitGameplay(void* data);
	GAMEPLAY_API const void* Theater_GetActorsRegistry();
	GAMEPLAY_API void Theater_ShutdownGameplay();
	GAMEPLAY_API void Theater_UpdateScene(float deltaTime);
	GAMEPLAY_API void Theater_RenderScene(void* data);
}


int Theater_InitGameplay(void* data)
{
	return 0;
}

const void* Theater_GetActorsRegistry()
{
	return &Theater::ActorRegistry::Get()->GetActorsRegistry();
}

void Theater_ShutdownGameplay()
{
	//Theater::ActorRegistry::Get()->Destroy();
}

void Theater_UpdateScene(float deltaTime)
{

}

void Theater_RenderScene(void* data)
{

}