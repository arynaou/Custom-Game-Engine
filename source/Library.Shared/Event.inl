#include "Event.h"

namespace FieaGameEngine
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>)

	template <typename T>
	Event<T>::Event(T& message): EventPublisher(_eventSubscriber),
		_message(message)
	{
	}

	template <typename T>
	Event<T>::~Event()
	{
	}

	template <typename T>
	inline void Event<T>::Subscribe(EventSubscriber& subscribeEvent)
	{
		_eventSubscriber.EmplaceBack(subscribeEvent);
	}

	template <typename T>
	inline void Event<T>::Unsubscribe(EventSubscriber& unsubscribeEvent)
	{
		for (Vector<std::reference_wrapper<EventSubscriber>>::Iterator it= _eventSubscriber.begin(); it!=_eventSubscriber.end(); ++it)
		{
			if (&((*it).get()) == &unsubscribeEvent)
			{
				_eventSubscriber.Remove(it);
			}
		}
	}

	template <typename T>
	inline void Event<T>::UnsubscribeAll()
	{
		_eventSubscriber.Clear();
		_eventSubscriber.ShrinkToFit();
	}

	template <typename T>
	inline const T& Event<T>::Message() const
	{
		return _message;
	}

	template <typename T>
	Event<T>::Event(const Event<T>& rhs) : EventPublisher(_eventSubscriber)
	{
		if (this != &rhs)
		{
			_message = rhs._message;
			_eventSubscriber = rhs._eventSubscriber;
		}
	}

	template <typename T>
	Event<T>& Event<T>::operator=(const Event<T>& rhs)
	{
		if (this != &rhs)
		{
			_eventSubscriber.Clear();

			_message = rhs._message;
			_eventSubscriber = rhs._eventSubscriber;
		}

		return *this;
	}

	template <typename T>
	Event<T>::Event(Event&& rhs) noexcept: EventPublisher(_eventSubscriber)
	{
		if (this != &rhs)
		{
			_message = rhs._message;
			_eventSubscriber = std::move(rhs._eventSubscriber);
		}
	}

	template <typename T>
	Event<T>& Event<T>::operator=(Event<T>&& rhs) noexcept
	{
		if (this != &rhs)
		{
			_eventSubscriber.Clear();

			_message = std::move(rhs._message);
			_eventSubscriber = std::move(rhs._eventSubscriber);
		}

		return *this;
	}
}