#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<std::reference_wrapper<EventSubscriber>>& eventSubscriber)
		: _eventSubscriber(eventSubscriber)
	{
	}

	void EventPublisher::Deliver()
	{
		for (size_t i = 0; i < _eventSubscriber.Size(); ++i)
		{
			_eventSubscriber[i].get().Notify(*this);
		}
	}

	EventPublisher::~EventPublisher()
	{
	}
}