#include <iostream>

#include "Hero.hpp"

using namespace Theater;

THT_REGISTER_ACTORCLASS(Hero, 4523456234)
Hero::Hero()
{
	
}

void Hero::OnGameReady()
{
	std::cout << "Hero Instanced\n";
	m_Model.Load("C://Users//andre//Downloads//Vanguard By T. Choonyung.fbx"); // move to bin dir
	if (m_Model.GetError().empty())
	{
		std::cout << "Model Loaded\n";
	}

	m_Transform.Position.y = -130.0f;
}

void Hero::OnUpdate(float dt)
{
	static float sRotFactor = 2.0f;
	m_Transform.Rotation.y += sRotFactor * dt;
}

void Hero::OnDraw(Shader& shader)
{
	Actor::OnDraw(shader);
}