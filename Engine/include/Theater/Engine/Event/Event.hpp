#pragma once

#include <cstdint>
#include <memory>
#include <functional>

#include "../Utility/MemoryPool.hpp"

#pragma region Event
namespace Theater
{
	using EventID = uint64_t;
	using EventListenerID = uint64_t;

	namespace Detail
	{
		inline EventID IncrEventTypes()
		{
			static EventID EventTypes = 1ull; // 0 is Invalid
			return EventTypes++;
		}
	}

	struct Event
	{
		virtual ~Event() = default;

		void* pSender = nullptr;
		virtual EventID GetID() const = 0;
	};
	using EventListenerFn = std::function<void(const Event*)>;

	template<typename TEvnt>
	inline const TEvnt* EventCast(const Event* pEvent)
	{
		return static_cast<const TEvnt*>(pEvent);
	}
}

// THT_EVENT_OBJ macro must be public in the custom event class declaration
#define THT_EVENT_OBJ \
static inline const Theater::EventID s_ID = Theater::Detail::IncrEventTypes(); \
Theater::EventID GetID() const override { return s_ID; } 

#pragma endregion