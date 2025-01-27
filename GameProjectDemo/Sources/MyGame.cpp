#include "Hero.hpp"

#include "MyGame.hpp"

using namespace Theater;

THT_GAME(MyGame)
MyGame::MyGame()
{
}

void MyGame::OnGameReady()
{
	SetProjectionMat(BuildPerspective(ToRad(60.f), 1920.f / 1080.f, 0.1, 1000.f));
	SetViewMat(Translate(IMat4, Vector3(0, 0, -100.f)));
}

void MyGame::OnUpdate(float deltaTime)
{
	Game::OnUpdate(deltaTime);

	// Custom Updating
	{

	}
}
