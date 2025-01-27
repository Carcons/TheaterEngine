#pragma once

#include <bitset>
#include <cstdint>

#include "../System.hpp"
#include "../Event/Event.hpp"
#include "../Event/EventDispatcher.hpp"
#include "../EngineAPI.hpp"

#pragma region Constants
namespace Theater
{
	constexpr inline size_t MAX_JOYSTICKS_ID = 4;
	constexpr inline size_t JOYSTICKS_BUTTONS_COUNT = 14;
	constexpr inline size_t JOYSTICK_TRIGGERS_COUNT = 2;
	constexpr inline size_t JOYSTICK_THUMBSTICKS_COUNT = 2;

	using JoystickID = uint16_t;
}
#pragma endregion

#pragma region Enums

namespace Theater
{
	enum class JoystickButton : uint16_t
	{
		DPAD_UP = 0,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		START,
		BACK,
		L3,
		R3,
		LB,
		RB,
		A,
		B,
		X,
		Y
	};

	enum class JoystickThumbstick : uint16_t
	{
		LTHUMBSTICK_UP = 14,
		LTHUMBSTICK_DOWN,
		LTHUMBSTICK_LEFT,
		LTHUMBSTICK_RIGHT,
		LTHUMBSTICK_UPLEFT,
		LTHUMBSTICK_UPRIGHT,
		LTHUMBSTICK_DOWNLEFT,
		LTHUMBSTICK_DOWNRIGHT,
		RTHUMBSTICK_UP,
		RTHUMBSTICK_DOWN,
		RTHUMBSTICK_LEFT,
		RTHUMBSTICK_RIGHT,
		RTHUMBSTICK_UPLEFT,
		RTHUMBSTICK_UPRIGHT,
		RTHUMBSTICK_DOWNLEFT,
		RTHUMBSTICK_DOWNRIGHT
	};

	enum class JoystickTrigger : uint16_t
	{
		LT = 30,
		RT
	};

	enum class JoystickThumbAxis : uint16_t
	{
		LTHUMB_X,
		LTHUMB_Y,
		RTHUMB_X,
		RTHUMB_Y
	};
}

#pragma endregion

#pragma region JoystickSystem

namespace Theater
{
	class ENGINE_API Joystick : public System<Joystick>, public EventDispatcher
	{
	public:

		Joystick();
		virtual ~Joystick() = default;

		Joystick(const Joystick&) = delete;
		Joystick& operator = (const Joystick) = delete;

	public:

		virtual void SetVibration(JoystickID id, bool active) = 0;
		virtual void UpdateState() = 0;

	public:

		bool	IsButtonPressed(JoystickID id, JoystickButton button) const;
		bool	IsButtonReleased(JoystickID id, JoystickButton button) const;
		float	GetTriggerValue(JoystickID id, JoystickTrigger trigger) const;
		float	GetThumbstickValue(JoystickID id, JoystickThumbAxis thumbAxis) const;
		bool	IsConnected(JoystickID id) const;

	protected:

		enum _JoystickState : size_t
		{
			CURRENT_STATE,
			OLD_STATE,

			JOYSTICK_STATES_COUNT
		};

		using ButtonsBitset = std::bitset<JOYSTICKS_BUTTONS_COUNT>;
		struct _JoystickData
		{
			bool connected;
			JoystickID id;
			ButtonsBitset buttons[JOYSTICK_STATES_COUNT];
			float lThumbX;
			float lThumbY;
			float rThumbX;
			float rThumbY;
			float lTrigger;
			float rTrigger;
		} m_joysticks[MAX_JOYSTICKS_ID];

	};

	Joystick* NewJoystick();
	void DeleteJoystick(Joystick* ptr);
}


#pragma endregion

#pragma region JoystickEvents
namespace Theater
{
	struct JoystickConnection : public Event
	{
		THT_EVENT_OBJ

		JoystickConnection(JoystickID t_id, bool t_connected) :
			connected(t_connected),
			id(t_id)
		{}

		bool		connected;
		JoystickID	id;
	};

	struct JoystickPress : public Event
	{
		THT_EVENT_OBJ

		JoystickPress(JoystickID t_id, uint16_t t_value) :
			value(t_value),
			id(t_id)
		{}

		JoystickID	id;
		uint16_t	value;
	};

	struct JoystickRelease : public Event
	{
		THT_EVENT_OBJ

		JoystickRelease(JoystickID t_id, uint16_t t_value) :
			value(t_value),
			id(t_id)
		{}

		JoystickID	id;
		uint16_t	value;
	};

	struct JoystickRepeat : public Event
	{
		THT_EVENT_OBJ

		JoystickRepeat(JoystickID t_id, uint16_t t_value) :
			value(t_value),
			id(t_id)
		{}

		JoystickID	id;
		uint16_t	value;
	};

	struct JoystickTriggerPressed : public Event
	{
		THT_EVENT_OBJ

		JoystickTriggerPressed(JoystickID t_id, JoystickTrigger t_trigger) :
			id(t_id),
			trigger(t_trigger)
		{}

		JoystickID			id;
		JoystickTrigger		trigger;
	};

	struct JoystickTriggerReleased : public Event
	{
		THT_EVENT_OBJ

		JoystickTriggerReleased(JoystickID t_id, JoystickTrigger t_trigger) :
			id(t_id),
			trigger(t_trigger)
		{}

		JoystickID			id;
		JoystickTrigger		trigger;
	};
}
#pragma endregion