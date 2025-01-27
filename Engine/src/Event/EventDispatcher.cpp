#include <Theater/Engine/Event/EventDispatcher.hpp>

namespace
{
	Theater::EventListenerID IncrEventListenerID()
	{
		static Theater::EventListenerID sID = 1ull; // 0 is invalid
		return sID++;
	}
}

namespace Theater
{
	void EventDispatcher::ReserveEventListenersCapacity(size_t capacity)
	{
		m_EventListeners.reserve(capacity);
		m_EventListenersSet.reserve(capacity);
	}

	EventListenerID EventDispatcher::SubscribeEventListener(EventListenerFn&& fn)
	{
		const EventListenerID ListenerID = ::IncrEventListenerID();

		m_EventListenersSet.insert(ListenerID);
		m_EventListeners.push_back(EventListener(ListenerID, std::forward<EventListenerFn>(fn)));

		return ListenerID;
	}

	void EventDispatcher::SetEventListenerCallback(EventListenerID id, EventListenerFn&& fn)
	{
		if (!m_EventListenersSet.count(id))
			return;

		auto it = std::find(m_EventListeners.begin(), m_EventListeners.end(), id);
		if (it != m_EventListeners.end())
		{
			it->Callback = std::forward<EventListenerFn>(fn);
		}
	}

	void EventDispatcher::UnSubscribeEventListener(EventListenerID id)
	{
		if (m_EventListenersSet.count(id))
		{
			m_EventListeners.erase(std::find(m_EventListeners.begin(), m_EventListeners.end(), id));
			m_EventListenersSet.erase(id);
		}
	}

	void EventDispatcher::PublishEvent(std::shared_ptr<Event> pEvent)
	{
		m_EventsQueue.push(pEvent);
	}

	void EventDispatcher::DispatchEvents()
	{
		while (!m_EventsQueue.empty())
		{
			const std::shared_ptr<Event> pEvent = m_EventsQueue.front();
			m_EventsQueue.pop();

			if (!pEvent)
				continue;

			for (auto& listener : m_EventListeners)
			{
				if (listener.Callback)
					listener.Callback(pEvent.get());
			}
		}
	}
}