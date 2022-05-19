#include "pch.h"
#include "EventQueue.h"
#include <algorithm>

namespace FieaGameEngine
{
	bool EventQueue::QueueEntry::IsExpired(const GameTime& gameTime) const
	{
		return (gameTime.CurrentTime() > _expirationTime);
	}

	EventQueue::QueueEntry::QueueEntry(std::shared_ptr<EventPublisher> eventPublisher, std::chrono::high_resolution_clock::time_point expirationTime):
		_eventPublisher(eventPublisher), _expirationTime(expirationTime)
	{
	}
	
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> eventPublisher, const GameTime& gameTime, std::chrono::milliseconds delay)
	{
		QueueEntry queueEntry = QueueEntry(eventPublisher, gameTime.CurrentTime() + delay);
		if (_isUpdate)
		{
			_pendingVector.PushBack(queueEntry);
		}
		else
		{
			_eventVector.PushBack(queueEntry);
		}
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		_isUpdate = true;

		auto it = std::partition(_eventVector.begin(), _eventVector.end(), [gameTime](QueueEntry& entry)
			{
				return !(entry.IsExpired(gameTime));
			});

		for (Vector<QueueEntry>::Iterator expiredIt = it; expiredIt != _eventVector.end(); ++expiredIt)
		{
			(*expiredIt)._eventPublisher->Deliver();
		}

		_eventVector.Remove(it, _eventVector.end());

		_isUpdate = false;
		
		for (size_t i = 0; i < _pendingVector.Size(); ++i)
		{
			_eventVector.PushBack(_pendingVector[i]);
		}

		_pendingVector.Clear();

		if (_pendingClear)
		{
			Clear();
			_pendingClear = false;
		}		
	}

	void EventQueue::Clear()
	{
		if (_isUpdate)
		{
			_pendingClear = true;
		}
		else
		{
			_eventVector.Clear();
		}
	}

	const bool EventQueue::IsEmpty() const
	{
		return (_eventVector.IsEmpty());
	}


	const size_t EventQueue::Size() const
	{
		return _eventVector.Size();
	}

	EventQueue::~EventQueue()
	{
		Clear();
	}
}