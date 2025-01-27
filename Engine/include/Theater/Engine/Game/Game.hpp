#pragma once

#include <vector>
#include <unordered_set>
#include <string>

#include "../Math/Mat4.hpp"
#include "../EngineAPI.hpp"
#include "../GameplayAPI.hpp"
#include "../System.hpp"

#include "../Utility/UUID.hpp"

namespace Theater
{
	class Actor;
	class WindowSystem;
	class Renderer;
	class Shader;
	class Joystick;

	using CommandLineArgs = std::vector<std::string>;

	namespace Detail
	{
		inline constexpr char* GAME_LIBRARY = 
		#if defined(_WIN32)
			"TheaterGame.dll";
		#else
			"TheaterGame.so"
		#endif
		
		inline constexpr char* GAME_DATA = "TheaterGame.tht";

		struct GameplayInitData;
		GAMEPLAY_API void GameInitializer(const GameplayInitData* pData);
		GAMEPLAY_API void* GetGameData();
		GAMEPLAY_API void WriteGameData();
		GAMEPLAY_API void GameOnUpdate(float dt);
		GAMEPLAY_API void GameOnDraw();
	}

	class ENGINE_API Game : public System<Game>
	{
		friend void TheaterMain(const CommandLineArgs&);
		friend void Detail::GameInitializer(const GameplayInitData*);
		friend void* Detail::GetGameData();
		friend void Detail::WriteGameData();
		friend void Detail::GameOnUpdate(float); 
		friend void Detail::GameOnDraw(); 

	public:

		Game();
		virtual ~Game();

	public:

		void SetVSync(bool enable);

	public:

		void AddActor(Actor* pActor);
		void RemoveActor(uuid UUID);

		void SetProjectionMat(const Matrix4& projMat);
		void SetViewMat(const Matrix4& viewMat);

	public:

		Actor* GetActorByUUID(uuid UUID);
		const CommandLineArgs& GetCommandLineArgs() const { return m_CmdArgs; }

		const Matrix4& GetProjectionMat() const { return m_ProjMatrix; }
		const Matrix4& GetViewMat() const { return m_ViewMatrix; }

	protected:

		virtual void OnGameReady() {};
		virtual void OnUpdate(float deltaTime);

	private:

		void OnGameReady_External();

		void OnDraw();
		void RenderPass();

		void SerializeGameData();
		void DeserializeGameData();

	private:


		Joystick* m_pJoystick;
		Matrix4 m_ProjMatrix = IMat4;
		Matrix4 m_ViewMatrix = IMat4;

		void* m_pGameData;

		bool m_Iterating = false;
		std::vector<Actor*> m_Actors;
		std::vector<Actor*> m_PendingActors;
		std::unordered_set<Actor*> m_PendingRemove;

		CommandLineArgs m_CmdArgs;
	};

}