#pragma once

#include "Event.h"
#include "Foo.h"
#include "EventQueue.h"
#include "EventSubscriber.h"
#include "GameTime.h"

namespace UnitTests
{
	struct FooSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		bool WasNotified = false;
		int Data = 0;
	};

	struct FooCountSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;

		std::size_t Count = 0;
	};

	class EventEnqueFoo final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(EventEnqueFoo, FieaGameEngine::RTTI)

	public:
		explicit EventEnqueFoo(FieaGameEngine::EventQueue& eventQueue);

		FieaGameEngine::EventQueue& Queue();

	private:
		FieaGameEngine::EventQueue* _mEventQueue;
	};

	struct EventEnqueueSubscriber final : public FieaGameEngine::EventSubscriber
	{	
	public:
		void Notify(const FieaGameEngine::EventPublisher& e) override;

		std::size_t Count = 0;

	};

	struct EventQueueRef
	{
		FieaGameEngine::EventQueue& mEventQueue;
		FieaGameEngine::GameTime& mGametime;

		EventQueueRef(FieaGameEngine::EventQueue& e, FieaGameEngine::GameTime& g) : mEventQueue(e), mGametime(g) {};
	};

	struct TraverseSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;
	};

	struct ClearSubscriber final : public FieaGameEngine::EventSubscriber
	{
		void Notify(const FieaGameEngine::EventPublisher& event) override;
	};
}