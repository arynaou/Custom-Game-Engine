#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Event.h"
#include "FooEventSubscribers.h"
#include "EventQueue.h"
#include "GameState.h"
#include <chrono>

using namespace std::chrono_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(RTTITest)
		{
			Foo foo{1};
			Event<Foo> event(foo);

			RTTI* rtti = &event;
			Assert::IsTrue(rtti->Is(Event<Foo>::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));

			Event<Foo>* e = rtti->As<Event<Foo>>();
			Assert::IsNotNull(e);
			Assert::AreEqual(&event, e);

			EventPublisher* ep = rtti->As<EventPublisher>();
			Assert::IsNotNull(ep);
			Assert::AreEqual(&event, static_cast<Event<Foo>*>(ep));
			Assert::IsTrue(rtti->Is(ep->TypeIdInstance()));

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventPublisherRTTI)
		{
			Foo foo{ 1 };
			Event<Foo> event(foo);
			EventPublisher* eventP = static_cast<EventPublisher*>(&event);

			RTTI* rtti = eventP;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("EventPublisher"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(eventP->TypeIdInstance(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			EventPublisher* a = rtti->As<EventPublisher>();
			Assert::IsNotNull(a);
			Assert::AreEqual(eventP, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(TestEventDeliver)
		{
			Foo foo{ 1 };
			FooSubscriber fooSubscriber;
			Event<Foo> event(foo);
			Assert::AreEqual(foo, event.Message());

			Assert::IsFalse(fooSubscriber.WasNotified);
			event.Deliver();
			Assert::IsFalse(fooSubscriber.WasNotified);

			Event<Foo>::Subscribe(fooSubscriber);
			event.Deliver();
			Assert::IsTrue(fooSubscriber.WasNotified);
			Assert::AreEqual(event.Message().Data(), fooSubscriber.Data);

			fooSubscriber.WasNotified = false;
			Event<Foo>::Unsubscribe(fooSubscriber);
			event.Deliver();
			Assert::AreEqual(fooSubscriber.WasNotified, false);

			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gameTime;
			Foo foo{ 1 };
			FooSubscriber fooSubscriber;
			EventQueue eventQueue;

			std::shared_ptr<Event<Foo>> event = std::make_shared<Event<Foo>>(foo);
			Assert::AreEqual(event->Message(), foo);

			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			Assert::AreEqual(size_t(0), eventQueue.Size());
			eventQueue.Enqueue(event, gameTime);
			Assert::AreEqual(size_t(1), eventQueue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber.WasNotified);
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			fooSubscriber.WasNotified = false;
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(size_t(0), eventQueue.Size());

			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point());
			eventQueue.Enqueue(event, gameTime, 1s);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(2s));
			eventQueue.Update(gameTime);
			Assert::IsTrue(fooSubscriber.WasNotified);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueMultupleEvents)
		{
			FooCountSubscriber subscriber;
			Event<Foo>::Subscribe(subscriber);
			Foo foo{ 1 };
			Foo foo1{ 1 };
			Foo foo2{ 1 };
			Foo foo3{ 1 };

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo);
			std::shared_ptr<Event<Foo>> event2 = std::make_shared<Event<Foo>>(foo1);
			std::shared_ptr<Event<Foo>> event3 = std::make_shared<Event<Foo>>(foo2);
			std::shared_ptr<Event<Foo>> event4 = std::make_shared<Event<Foo>>(foo3);

			GameTime gameTime;
			EventQueue eventQueue;
			eventQueue.Enqueue(event1, gameTime, 750ms);
			eventQueue.Enqueue(event2, gameTime);
			eventQueue.Enqueue(event3, gameTime);
			eventQueue.Enqueue(event4, gameTime, 1s);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(2), subscriber.Count);
			
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1500ms));
			eventQueue.Update(gameTime);
			Assert::AreEqual(size_t(4), subscriber.Count);

			Assert::IsTrue(eventQueue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
		}

		TEST_METHOD(CopySemantics)
		{
			Foo foo{ 1 };
			Foo foo1{ 2 };

			{
				Event<Foo> event(foo);
				Event<Foo> anotherEvent(event);
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}

			{
				Event<Foo> event(foo);
				Event<Foo> anotherEvent(foo1);
				anotherEvent = event;
				Assert::AreNotSame(event.Message(), anotherEvent.Message());
				Assert::AreEqual(event.Message(), anotherEvent.Message());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			Foo foo{ 1 };
			Foo foo1{ 2 };

			{
				Event<Foo> event(foo);
				Event<Foo> anotherEvent(std::move(event));
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
			}

			{
				Event<Foo> event(foo);
				Event<Foo> anotherEvent(foo1);
				anotherEvent = std::move(event);
				Assert::IsFalse(&event.Message() == &anotherEvent.Message());
			}
		}

		TEST_METHOD(EventQueueClear)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			Foo foo{ 1 };
			Assert::AreEqual(size_t(0), eventQueue.Size());

			std::shared_ptr<Event<Foo>> event1 = std::make_shared<Event<Foo>>(foo);
			
			const size_t eventCount = 4;
			for (size_t i = 0; i < eventCount; ++i)
			{
				eventQueue.Enqueue(event1, gameTime);
			}

			Assert::AreEqual(eventCount, eventQueue.Size());
			eventQueue.Clear();
			Assert::AreEqual(size_t(0), eventQueue.Size());
		}

		TEST_METHOD(PendingEvent)
		{
			GameTime gameTime;
			TraverseSubscriber traverseSubscriber;
			EventQueue eventQueue;
			
			EventQueueRef e = EventQueueRef(eventQueue, gameTime);
			std::shared_ptr<Event<EventQueueRef>> event = std::make_shared<Event<EventQueueRef>>(e);

			Event<EventQueueRef>::Subscribe(traverseSubscriber);
			eventQueue.Enqueue(event, gameTime);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Event<EventQueueRef>::UnsubscribeAll();
		}

		TEST_METHOD(ClearEvent)
		{
			GameTime gameTime;
			ClearSubscriber clearSubscriber;
			EventQueue eventQueue;

			EventQueueRef e = EventQueueRef(eventQueue, gameTime);
			std::shared_ptr<Event<EventQueueRef>> event = std::make_shared<Event<EventQueueRef>>(e);

			Event<EventQueueRef>::Subscribe(clearSubscriber);
			eventQueue.Enqueue(event, gameTime);
			gameTime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			eventQueue.Update(gameTime);
			Event<EventQueueRef>::UnsubscribeAll();
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState EventTests::_startMemState;
}