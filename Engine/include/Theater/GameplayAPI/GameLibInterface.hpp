#pragma once

#include "../Engine/Game/ActorRegistry.hpp"
#include "../Engine/Game/Game.hpp"
#include "../Engine/Graphics/RenderingSystem.hpp"
#include "../Engine/Graphics/Vertex.hpp"
#include "../Engine/GameplayAPI.hpp"

extern "C"
{
	GAMEPLAY_API int Theater_InitGameplay(void* pData);
	GAMEPLAY_API void Theater_ShutdownGameplay();
	GAMEPLAY_API void* Theater_GetGameData();
	GAMEPLAY_API void Theater_WriteGameData();
	GAMEPLAY_API void* Theater_GetActorsClassData();
	GAMEPLAY_API void Theater_FreeActorsClassData(void* ptr);
	GAMEPLAY_API void* Theater_NewActorInstance(uint64_t classUUID);
	GAMEPLAY_API void* Theater_GetActorInstance(uint64_t actorUUID);
	GAMEPLAY_API void Theater_DeleteActorInstance(uint64_t actorUUID);
	GAMEPLAY_API void Theater_UpdateScene(float deltaTime);
	GAMEPLAY_API void Theater_RenderScene(void* pRenderingTarget, void* pProjMat, void* pViewMat);
}

using namespace Theater;
using namespace Theater::Detail;

int Theater_InitGameplay(void* pData)
{
	GameInitializer(nullptr);
	return 0;
}

void Theater_ShutdownGameplay()
{
	ActorRegistry::Get()->Destroy();
	delete Game::GetPtr();
}

void* Theater_GetGameData()
{
	return GetGameData();
}

void Theater_WriteGameData()
{
	WriteGameData();
}

void Theater_DeleteActorInstance(uint64_t actorUUID)
{
	Game::Get().RemoveActor(actorUUID);
}

void* Theater_NewActorInstance(uint64_t classUUID)
{
	Actor* const pActor = ActorRegistry::Get()->CreateInstance(classUUID);
	Game::Get().AddActor(pActor);

	return pActor;
}

void* Theater_GetActorInstance(uint64_t actorUUID)
{
	return Game::Get().GetActorByUUID(actorUUID);
}

void* Theater_GetActorsClassData()
{
	return ActorRegistry::Get()->GetActorsClassData();
}

void Theater_FreeActorsClassData(void* ptr)
{
	delete ptr;
}

void Theater_UpdateScene(float deltaTime)
{
	GameOnUpdate(deltaTime);
}

void Theater_RenderScene(void* pRenderingTarget, void* ProjMat, void* ViewMat)
{
	RenderingTarget* const pTarget = reinterpret_cast<RenderingTarget*>(pRenderingTarget);
	Matrix4* pProjection = reinterpret_cast<Matrix4*>(ProjMat);
	Matrix4* pView = reinterpret_cast<Matrix4*>(ViewMat);

	if (!pTarget)
	{
		return;
	}

	Game::Get().SetProjectionMat(*pProjection);
	Game::Get().SetViewMat(*pView);

	RenderingSystem* const pRenderer = RenderingSystem::GetPtr();
;
	GameOnDraw();
}