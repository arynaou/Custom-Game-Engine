#pragma once
#include "RTTI.h"
#include "EventSubscriber.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);

	public:
		EventPublisher(const EventPublisher&) = default;
		EventPublisher(EventPublisher&&) noexcept = default;
		EventPublisher& operator=(const EventPublisher&)=default;
		EventPublisher& operator=(EventPublisher&&) noexcept = default;
		virtual ~EventPublisher();
		
		/// <summary>
		/// Deliver and notify all the event in the vector
		/// </summary>
		void Deliver();
		
	protected:
		/// <summary>
		/// constructor of EventPublisher
		/// </summary>
		/// <param name="">the vector of event subscriber</param>
		EventPublisher(Vector<std::reference_wrapper<EventSubscriber>>&);

		Vector<std::reference_wrapper<EventSubscriber>>& _eventSubscriber;
	};
}