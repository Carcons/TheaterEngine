#include <iostream>

#include "Hero_Female.hpp"

using namespace Theater;

THT_REGISTER_ACTORCLASS(Hero_Female, 8329839238)
Hero_Female::Hero_Female()
{
	
}

void Hero_Female::OnGameReady()
{
	std::cout << "Hero Instanced\n";
	m_Model.Load("C://Users//andre//Downloads//female.fbx");
	if (m_Model.GetError().size() == 0)
	{
		std::cout << "Model Loaded\n";
	}

	m_InputListener = Joystick::Get().SubscribeEventListener([&](const Event* pEvent) {
		std::cout << pEvent->GetID() << "\n";
		if (pEvent->GetID() == JoystickConnection::s_ID)
		{
			std::cout << "Joystick Connected\n";
		}
		else if (pEvent->GetID() == JoystickPress::s_ID)
		{
			const auto* pPressEvent = EventCast<JoystickPress>(pEvent);
			std::cout << pPressEvent->value << '\n';
		}
	});
}

void Hero_Female::OnUpdate(float dt)
{
	//m_Transform.Rotation.y -= 0.005;
}

void Hero_Female::OnDraw(Shader& shader)
{
	Actor::OnDraw(shader);
}