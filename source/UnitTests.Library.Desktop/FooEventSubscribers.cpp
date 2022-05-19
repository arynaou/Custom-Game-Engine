#include "pch.h"
#include "FooEventSubscribers.h"

using namespace std;
using namespace FieaGameEngine;

namespace UnitTests
{
	void FooSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr)
		{
			const Foo& foo = e->Message();
			Data = foo.Data();
		}

		WasNotified = true;
	}

	void FooCountSubscriber::Notify(const EventPublisher& event)
	{
		const Event<Foo>* e = event.As<Event<Foo>>();
		if (e != nullptr)
		{
			++Count;
		}
	}

	void TraverseSubscriber::Notify(const EventPublisher& event)
	{
		const Event<EventQueueRef>* e = event.As<Event<EventQueueRef>>();
		if (e != nullptr)
		{
			EventQueueRef newref = e->Message();
			std::shared_ptr<Event<EventQueueRef>> anotherE = std::make_shared<Event<EventQueueRef>>(newref);
			e->Message().mEventQueue.Enqueue(anotherE, e->Message().mGametime);
		}
	}

	void ClearSubscriber::Notify(const EventPublisher& event)
	{
		const Event<EventQueueRef>* e = event.As<Event<EventQueueRef>>();
		if (e != nullptr)
		{
			EventQueueRef newref = e->Message();
			std::shared_ptr<Event<EventQueueRef>> anotherE = std::make_shared<Event<EventQueueRef>>(newref);
			e->Message().mEventQueue.Clear();
		}
	}
}