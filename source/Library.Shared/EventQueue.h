#pragma once
#include "EventPublisher.h"
#include "GameTime.h"
#include "Event.h"
#include "Vector.h"
#include <chrono>

using namespace std::chrono_literals;

namespace FieaGameEngine
{
	class EventQueue
	{
		struct QueueEntry
		{
			/// <summary>
			/// if the Queue is expired
			/// </summary>
			/// <param name="">Gametime</param>
			/// <returns>if the Event is expired</returns>
			bool IsExpired(const GameTime& gametime) const;

			/// <summary>
			/// the event publisher pointer
			/// </summary>
			std::shared_ptr<EventPublisher> _eventPublisher;

			/// <summary>
			/// the expiration time (currentTime + delayTime)
			/// </summary>
			std::chrono::high_resolution_clock::time_point _expirationTime;

			/// <summary>
			/// constructor of QueueEntry
			/// </summary>
			/// <param name="eventPublisher">eventPublisher</param>
			/// <param name="expirationTime">expiration Time</param>
			QueueEntry(std::shared_ptr<EventPublisher> eventPublisher, std::chrono::high_resolution_clock::time_point expirationTime);
		};

	public:
		/// <summary>
		/// The Enqueue constructor
		/// </summary>
		/// <param name="event">the shared pointer event publisher</param>
		/// <param name="">the game time</param>
		/// <param name="delayTime">the delay time</param>
		void Enqueue(std::shared_ptr<EventPublisher> event, const GameTime&, std::chrono::milliseconds delayTime = 0s);
		virtual ~EventQueue();

		/// <summary>
		/// Update function will Deliver the expired event
		/// </summary>
		/// <param name="">gametime</param>
		void Update(const GameTime&);

		/// <summary>
		/// Clear the vector
		/// </summary>
		void Clear();

		/// <summary>
		/// if the vector is empty
		/// </summary>
		/// <returns>if it's empty</returns>
		const bool IsEmpty() const;

		/// <summary>
		/// return the vector of size
		/// </summary>
		/// <returns>the size of the event vector</returns>
		const std::size_t Size() const;

	protected:
		Vector<QueueEntry> _eventVector;
		Vector<QueueEntry> _pendingVector;
		bool _isUpdate{false};
		bool _pendingClear{ false };
	};
};