#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <unordered_set>
#include <functional>

#include "../Engine/Game/ActorRegistry.hpp"
#include "../Engine/Game/Game.hpp"
#include "../Engine/Utility/UUID.hpp"
#include "../Engine/Actor/Actor.hpp"
#include "../Engine/Graphics/Shader.hpp"
#include "../Engine/Graphics/RenderingSystem.hpp"
#include "../Engine/Graphics/RenderingTarget.hpp"
#include "../Engine/Graphics/Mesh.hpp"
#include "../Engine/Math/Mat4.hpp"
#include "../Engine/Graphics/Model.hpp"
#include "../Engine/Graphics/Texture.hpp"
#include "../Engine/Platform/Joystick.h"
#include "../Engine/GameplayAPI.hpp"
#include "../Engine/Event/Event.hpp"
#include "../Engine/Event/EventDispatcher.hpp"

#pragma region GameLibInterface
namespace Theater
{
	namespace Detail
	{
		struct GameInterfaceFunctions
		{
			std::function<void* ()> NewGame;
			std::function<void (void*)> FreeGame;
			std::function<int(void*)> Init;
			std::function<void()> Shutdown;
			std::function<void*()> GetGameData;
			std::function<void()> WriteGameData;
			std::function<void* ()> GetActorsClassData;
			std::function<void (void*)> FreeActorsClassData;
			std::function<void*(uint64_t)> NewActorInstance;
			std::function<void*(uint64_t)> GetActorInstance;
			std::function<void(uint64_t)> DeleteActorInstance;
			std::function<void(float)> UpdateScene;
			std::function<void(void*, void*, void*)> RenderScene;
		};

		namespace GameplayFunctions
		{
			inline constexpr char* THEATER_INIT = "__Theater_Init";
			inline constexpr char* THEATER_SHUTDOWN = "__Theater_Shutdown";
			inline constexpr char* THEATER_GETGAMEDATA = "__Theater_GetGameData";
		}
	}
}
#pragma endregion


#define THT_GAME(ClassName)  \
	extern "C" GAMEPLAY_API void __Theater_FreeGame(void* ptr) { delete ptr; } \
	extern "C" GAMEPLAY_API void* __Theater_NewGame() { return new ClassName; }

namespace Theater
{
	namespace Detail
	{
		namespace GameplayFunctions
		{
			inline constexpr char* THEATER_NEWGAME = "__Theater_NewGame";
			inline constexpr char* THEATER_FREEGAME = "__Theater_FreeGame";
		}
	}
}