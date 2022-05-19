#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine
{
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);

	public:
		/// <summary>
		/// the constructor of event
		/// </summary>
		/// <param name="message">the message of the event</param>
		Event(T& message);

		/// <summary>
		/// the copy constructor
		/// </summary>
		/// <param name="">the Event intend to copy</param>
		Event(const Event&);

		/// <summary>
		/// the move constructor
		/// </summary>
		/// <param name="">the Event intend to move</param>
		Event(Event&&) noexcept;

		/// <summary>
		/// the copy assignment
		/// </summary>
		/// <param name="">the event intend to copy</param>
		/// <returns>the copied Event reference</returns>
		Event& operator=(const Event&);

		/// <summary>
		/// the move assignment
		/// </summary>
		/// <param name="">the event intend to move</param>
		/// <returns>the copied event reference</returns>
		Event& operator=(Event&&) noexcept;

		virtual ~Event() override;

		/// <summary>
		/// Subscribe an event
		/// </summary>
		/// <param name="">Event subscriber</param>
		static void Subscribe(EventSubscriber&);

		/// <summary>
		/// Unsubscribe an event
		/// </summary>
		/// <param name="">Event subscriber</param>
		static void Unsubscribe(EventSubscriber&);

		/// <summary>
		/// Unsubscribe all
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// return the Message it stored
		/// </summary>
		/// <returns>the message</returns>
		const T& Message() const;

	protected:
		T _message;
		inline static Vector<std::reference_wrapper<EventSubscriber>> _eventSubscriber;
	};
}
#include "Event.inl"