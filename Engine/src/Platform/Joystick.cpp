#include <Theater/Engine/Platform/Joystick.h>

namespace Theater
{
	Joystick::Joystick()
	{
		for (JoystickID i = 0; i < MAX_JOYSTICKS_ID; i++)
		{
			m_joysticks[i] = {};
			m_joysticks[i].connected = false;
			m_joysticks[i].id = i;
		}
	}

	bool Joystick::IsButtonPressed(JoystickID id, JoystickButton button) const
	{
		assert(id < MAX_JOYSTICKS_ID);

		if (!m_joysticks[id].connected)
			return false;

		return m_joysticks[id].buttons[CURRENT_STATE].test(static_cast<std::size_t>(button));
	}

	bool Joystick::IsButtonReleased(JoystickID id, JoystickButton button) const
	{
		assert(id < MAX_JOYSTICKS_ID);

		if (!m_joysticks[id].connected)
			return false;

		if (m_joysticks[id].buttons[CURRENT_STATE].test(static_cast<std::size_t>(button)) &&
			!m_joysticks[id].buttons[OLD_STATE].test(static_cast<std::size_t>(button)))
		{
			return true;
		}

		return false;
	}

	float Joystick::GetTriggerValue(JoystickID id, JoystickTrigger trigger) const
	{
		assert(id < MAX_JOYSTICKS_ID);

		if (!m_joysticks[id].connected)
			return 0.f;

		switch (trigger)
		{
		case JoystickTrigger::LT: return m_joysticks[id].lTrigger;
		case JoystickTrigger::RT: return m_joysticks[id].rTrigger;
		default:				  return 0.f;
		}
	}

	float Joystick::GetThumbstickValue(JoystickID id, JoystickThumbAxis thumbAxis) const
	{
		assert(id < MAX_JOYSTICKS_ID);

		if (!m_joysticks[id].connected)
			return 0.f;

		switch (thumbAxis)
		{
		case JoystickThumbAxis::LTHUMB_X: return m_joysticks[id].lThumbX;
		case JoystickThumbAxis::LTHUMB_Y: return m_joysticks[id].lThumbY;
		case JoystickThumbAxis::RTHUMB_X: return m_joysticks[id].rThumbX;
		case JoystickThumbAxis::RTHUMB_Y: return m_joysticks[id].rThumbY;
		default:						  return 0.f;
		}
	}

	bool Joystick::IsConnected(JoystickID id) const
	{
		assert(id < MAX_JOYSTICKS_ID);
		return m_joysticks[id].connected;
	}
}