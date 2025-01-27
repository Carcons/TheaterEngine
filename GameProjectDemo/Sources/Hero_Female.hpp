#pragma once
#include <Theater/GameplayAPI/TheaterGameplay.hpp>

class GAMEPLAY_API Hero_Female : public Theater::Actor
{
public:

	Hero_Female();
	~Hero_Female() {};
	
public:

	void OnUpdate(float dt) override;
	void OnDraw(Theater::Shader& shader);

private:

	void OnGameReady() override;

private:

	Theater::EventListenerID m_InputListener;
};