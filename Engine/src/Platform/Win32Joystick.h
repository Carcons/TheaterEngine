#pragma once

#if defined(_WIN32)
#include <XInput.h>

#include <Theater/Engine/Platform/Joystick.h>

namespace Theater
{
	class Win32Joystick final : public Joystick
	{

	public:

		Win32Joystick();
		~Win32Joystick() = default;

		Win32Joystick(const Win32Joystick&) = delete;
		Win32Joystick& operator = (const Win32Joystick&) = delete;

	public:

		void SetVibration(JoystickID id, bool active) override;
		void UpdateState() override;

	private:

		uint16_t MapFromXInputKeystroke(std::uint16_t button) const;

	};
}
#endif