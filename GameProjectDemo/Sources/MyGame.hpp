#pragma once

#include <Theater/GameplayAPI/TheaterGameplay.hpp>

class MyGame : public Theater::Game
{
public:

	MyGame();

private:

	void OnGameReady() override;

	// Custom Update
	void OnUpdate(float deltaTime) override;
};