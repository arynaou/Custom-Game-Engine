#pragma once

namespace FieaGameEngine
{
	class EventPublisher;

	class EventSubscriber
	{
	public:
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// the function should happened after event
		/// </summary>
		/// <param name="e">Event publisher reference</param>
		virtual void Notify(const EventPublisher& e) = 0;
	};
}