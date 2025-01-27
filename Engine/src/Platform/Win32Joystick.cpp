#if defined(_WIN32)

#include "Win32Library.hpp"
#include <Windows.h>

#include "Win32Joystick.h"

namespace Theater
{
	Joystick* NewJoystick()
	{
		return new Win32Joystick;
	}

	void DeleteJoystick(Joystick* ptr)
	{
		delete ptr;
	}
}

namespace Theater
{
	Win32Joystick::Win32Joystick()
	{
		for (JoystickID i = 0; i < MAX_JOYSTICKS_ID; i++)
		{
			m_joysticks[i] = {};
			m_joysticks[i].id = i;
		}
	}

	uint16_t Win32Joystick::MapFromXInputKeystroke(uint16_t button) const
	{
		switch (button)
		{
		case VK_PAD_DPAD_UP:                return static_cast<uint16_t>(JoystickButton::DPAD_UP);
		case VK_PAD_DPAD_DOWN:              return static_cast<uint16_t>(JoystickButton::DPAD_DOWN);
		case VK_PAD_DPAD_LEFT:              return static_cast<uint16_t>(JoystickButton::DPAD_LEFT);
		case VK_PAD_DPAD_RIGHT:             return static_cast<uint16_t>(JoystickButton::DPAD_RIGHT);
		case VK_PAD_START:                  return static_cast<uint16_t>(JoystickButton::START);
		case VK_PAD_BACK:                   return static_cast<uint16_t>(JoystickButton::BACK);
		case VK_PAD_LTHUMB_PRESS:           return static_cast<uint16_t>(JoystickButton::L3);
		case VK_PAD_RTHUMB_PRESS:           return static_cast<uint16_t>(JoystickButton::R3);
		case VK_PAD_LSHOULDER:              return static_cast<uint16_t>(JoystickButton::LB);
		case VK_PAD_RSHOULDER:              return static_cast<uint16_t>(JoystickButton::RB);
		case VK_PAD_A:                      return static_cast<uint16_t>(JoystickButton::A);
		case VK_PAD_B:                      return static_cast<uint16_t>(JoystickButton::B);
		case VK_PAD_X:                      return static_cast<uint16_t>(JoystickButton::X);
		case VK_PAD_Y:                      return static_cast<uint16_t>(JoystickButton::Y);
		case VK_PAD_LTRIGGER:               return static_cast<uint16_t>(JoystickTrigger::LT);
		case VK_PAD_RTRIGGER:               return static_cast<uint16_t>(JoystickTrigger::RT);
		case VK_PAD_LTHUMB_UP:              return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_UP);
		case VK_PAD_LTHUMB_DOWN:            return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_DOWN);
		case VK_PAD_LTHUMB_LEFT:            return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_LEFT);
		case VK_PAD_LTHUMB_RIGHT:           return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_RIGHT);
		case VK_PAD_LTHUMB_UPLEFT:          return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_UPLEFT);
		case VK_PAD_LTHUMB_UPRIGHT:         return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_UPRIGHT);
		case VK_PAD_LTHUMB_DOWNLEFT:        return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_DOWNLEFT);
		case VK_PAD_LTHUMB_DOWNRIGHT:       return static_cast<uint16_t>(JoystickThumbstick::LTHUMBSTICK_DOWNRIGHT);
		case VK_PAD_RTHUMB_UP:              return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_UP);
		case VK_PAD_RTHUMB_DOWN:            return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_DOWN);
		case VK_PAD_RTHUMB_LEFT:            return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_LEFT);
		case VK_PAD_RTHUMB_RIGHT:           return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_RIGHT);
		case VK_PAD_RTHUMB_UPLEFT:          return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_UPLEFT);
		case VK_PAD_RTHUMB_UPRIGHT:         return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_UPRIGHT);
		case VK_PAD_RTHUMB_DOWNLEFT:        return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_DOWNLEFT);
		case VK_PAD_RTHUMB_DOWNRIGHT:       return static_cast<uint16_t>(JoystickThumbstick::RTHUMBSTICK_DOWNRIGHT);
		default:                            return 0;
		}
	}

	void Win32Joystick::SetVibration(JoystickID id, bool active)
	{/*
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;
		XInputSetState(0, &vibration);*/
	}

	void Win32Joystick::UpdateState()
	{
		if (HasEventListeners())
		{
			DispatchEvents();
		}

		for (JoystickID id = 0; id < MAX_JOYSTICKS_ID; id++)
		{
			_JoystickData& joystick = m_joysticks[id];

			joystick.buttons[OLD_STATE] = joystick.buttons[CURRENT_STATE];
			joystick.buttons[CURRENT_STATE].reset();

			XINPUT_STATE currentXInputState = {};
			std::uint32_t currentConnResult = ::XInputGetState(id, &currentXInputState);

			// Connection Events
			if (currentConnResult == ERROR_SUCCESS)
			{
				if (!joystick.connected)
				{
					joystick.connected = true;

					PublishEvent(std::make_shared<JoystickConnection>(JoystickConnection(id, true)));
				}

				// Setup joystick state
				{
					auto& buttons = joystick.buttons[CURRENT_STATE];
					const auto xinputButtons = currentXInputState.Gamepad.wButtons;

					buttons[static_cast<std::size_t>(JoystickButton::DPAD_UP)] = xinputButtons & XINPUT_GAMEPAD_DPAD_UP;
					buttons[static_cast<std::size_t>(JoystickButton::DPAD_DOWN)] = xinputButtons & XINPUT_GAMEPAD_DPAD_DOWN;
					buttons[static_cast<std::size_t>(JoystickButton::DPAD_LEFT)] = xinputButtons & XINPUT_GAMEPAD_DPAD_LEFT;
					buttons[static_cast<std::size_t>(JoystickButton::DPAD_RIGHT)] = xinputButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
					buttons[static_cast<std::size_t>(JoystickButton::START)] = xinputButtons & XINPUT_GAMEPAD_START;
					buttons[static_cast<std::size_t>(JoystickButton::BACK)] = xinputButtons & XINPUT_GAMEPAD_BACK;
					buttons[static_cast<std::size_t>(JoystickButton::L3)] = xinputButtons & XINPUT_GAMEPAD_LEFT_THUMB;
					buttons[static_cast<std::size_t>(JoystickButton::R3)] = xinputButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
					buttons[static_cast<std::size_t>(JoystickButton::LB)] = xinputButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
					buttons[static_cast<std::size_t>(JoystickButton::RB)] = xinputButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
					buttons[static_cast<std::size_t>(JoystickButton::A)] = xinputButtons & XINPUT_GAMEPAD_A;
					buttons[static_cast<std::size_t>(JoystickButton::B)] = xinputButtons & XINPUT_GAMEPAD_B;
					buttons[static_cast<std::size_t>(JoystickButton::X)] = xinputButtons & XINPUT_GAMEPAD_X;
					buttons[static_cast<std::size_t>(JoystickButton::Y)] = xinputButtons & XINPUT_GAMEPAD_Y;

					joystick.lThumbX = currentXInputState.Gamepad.sThumbLX;
					joystick.lThumbY = currentXInputState.Gamepad.sThumbLY;
					joystick.rThumbX = currentXInputState.Gamepad.sThumbRX;
					joystick.rThumbY = currentXInputState.Gamepad.sThumbRY;
					joystick.rThumbY = currentXInputState.Gamepad.sThumbRY;
					joystick.lTrigger = currentXInputState.Gamepad.bLeftTrigger;
					joystick.rTrigger = currentXInputState.Gamepad.bRightTrigger;
				}


				// Press/Release Events
				if (joystick.connected)
				{
					XINPUT_KEYSTROKE keystroke{};

					if (::XInputGetKeystroke(id, 0, &keystroke) == ERROR_SUCCESS)
					{
						const uint16_t value = MapFromXInputKeystroke(keystroke.VirtualKey);
						std::shared_ptr<Event> pEvent = nullptr;

						if (keystroke.Flags & XINPUT_KEYSTROKE_KEYDOWN && !(keystroke.Flags & XINPUT_KEYSTROKE_REPEAT))
							pEvent = std::make_shared<JoystickPress>(id, value);
						else if (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP)
							pEvent = std::make_shared<JoystickRelease>(id, value);
						else if (keystroke.Flags & XINPUT_KEYSTROKE_REPEAT)
							pEvent = std::make_shared<JoystickRepeat>(id, value);

						PublishEvent(pEvent);
					}
				}

			} // Connection success
			else if (currentConnResult == ERROR_DEVICE_NOT_CONNECTED)
			{
				if (joystick.connected)
				{
					joystick = {};
					joystick.connected = false;
					joystick.id = id;

					PublishEvent(std::make_shared<JoystickConnection>(joystick.connected, id));
				}
			}
		} 
	}

}

#endif // _WIN32