#pragma once

#include <queue>
#include <memory>
#include <vector>
#include <unordered_set>

#include "Event.hpp"

#include "../EngineAPI.hpp"

namespace Theater
{
	class Event;
	class Application;

	class ENGINE_API EventDispatcher
	{
		struct EventListener
		{
			EventListenerID ID;
			EventListenerFn Callback;

			EventListener(EventListenerID id, EventListenerFn&& callback) :
				ID(id), Callback(callback)
			{}

			bool operator == (const EventListenerID& id) { return ID == id; }
		};

	public:

		EventDispatcher() = default;
		~EventDispatcher() = default;

		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator = (const EventDispatcher&) = delete;

		EventDispatcher(EventDispatcher&& other) noexcept
		{
			*this = std::forward<EventDispatcher>(other);
		}

		EventDispatcher& operator = (EventDispatcher&& other) noexcept
		{
			if (this != &other)
			{
				m_EventsQueue = std::move(other.m_EventsQueue);
				m_EventListeners = std::move(other.m_EventListeners);
				m_EventListenersSet = std::move(other.m_EventListenersSet);
			}

			return *this;
		}

	public:

		void ReserveEventListenersCapacity(size_t capacity);
		void SetEventListenerCallback(EventListenerID id, EventListenerFn&& fn);

		[[nodiscard]] EventListenerID SubscribeEventListener(EventListenerFn&& fn);
		void UnSubscribeEventListener(EventListenerID id);

	public:

		bool IsEventListenerSubscribed(EventListenerID id) const { return m_EventListenersSet.count(id); }
		bool HasEventListeners() const { return m_EventListenersSet.size() != 0; }
		bool HasPendingEvents() const { return m_EventsQueue.size() != 0; }

	protected:

		void PublishEvent(std::shared_ptr<Event> pEvent);
		void DispatchEvents();

	private:

		std::queue<std::shared_ptr<Event>> m_EventsQueue;
		std::vector<EventListener> m_EventListeners;
		std::unordered_set<EventListenerID> m_EventListenersSet;
	};
}