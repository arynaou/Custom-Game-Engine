#include "Vector.h"

namespace FieaGameEngine
{
	template <typename T>
	Vector<T>::Vector(std::size_t capacity, IncrementFunctor incrementFunctor) :
		_IncrementFunctor(incrementFunctor)
	{
		if (capacity > 0)
		{
			Reserve(capacity);
		}
	}

	template<typename T>
	Vector<T>::Vector(IncrementFunctor incrementFunctor) :
		_IncrementFunctor(incrementFunctor)
	{
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& other)
	{
		if (this != &other)
		{
			Reserve(other._capacity);
			for (size_t i = 0; i < other._size; ++i)
			{
				PushBack(*(other._data + i));
			}
		}
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& other)
	{
		if (this != &other)
		{
			Clear();

			Reserve(other._capacity);
			for (size_t i = 0; i < other._size; ++i)
			{
				PushBack(*(other._data + i));
			}
		}
		return *this;
	}

	template <typename T>
	T& Vector<T>::operator[](const size_t value)
	{
		if (value >= _size)
		{
			throw std::runtime_error("Invalid index.");
		}

		return *(_data + value);
	}

	template <typename T>
	const T& Vector<T>::operator[](const size_t value) const
	{
		if (value >= _size)
		{
			throw std::runtime_error("Invalid index.");
		}

		return *(_data + value);
	}

	template <typename T>
	T& Vector<T>::At(const size_t value)
	{
		if (value >= _size)
		{
			throw std::runtime_error("Invalid index.");
		}

		return *(_data + value);
	}

	template <typename T>
	const T& Vector<T>::At(const size_t value) const
	{
		if (value >= _size)
		{
			throw std::runtime_error("Invalid index.");
		}

		return *(_data + value);
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(_data);
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Vector.");
		}

		return *_data;
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Vector.");
		}

		return *_data;
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Vector.");
		}

		return *(_data + _size - 1);

	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return *(_data + _size - 1);
	}

	template <typename T>
	inline const std::size_t& Vector<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	inline const std::size_t& Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template <typename T>
	inline  typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		return EmplaceBack(value);
		/*if (_capacity <= _size)
		{
			if (_capacity == 0)
			{
				Reserve(1);
			}
			else
			{
				Reserve(_capacity * 2);
			}
		}

		new (_data + _size)T(value);
		_size++;*/
	}

	template <typename T>
	void Vector<T>::PopBack()
	{
		if (_size <= 0) return;

		(_data + _size - 1)->~T();
		_size--;
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			(_data + i)->~T();
		}
		_size = 0;
	}

	template <typename T>
	void Vector<T>::Reserve(std::size_t capacity)
	{
		if (capacity > _capacity)
		{
			T* buffer = reinterpret_cast<T*>(realloc(_data, sizeof(T) * capacity));
			_capacity = capacity;
			_data = buffer;
		}
	}

	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, const size_t index) :
		_owner(&owner), _index(index)
	{
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_index = other._index;

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_index = other._index;

		return *this;
	}

	template <typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		return !(operator==(other));
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return _owner == other._owner && _index == other._index;
	}

	template <typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return !(operator==(other));
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return _owner == other._owner && _index == other._index;
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner(other._owner), _index(other._index)
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const ConstIterator& other) :
		_owner(other._owner), _index(other._index)
	{
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_index < (_owner->_size))
		{
			++_index;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_index < (_owner->_size))
		{
			++_index;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}
		
		if (_index > 0)
		{
			--_index;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--(int)
	{
		Iterator temp = *this;
		operator--();
		return temp;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_index > 0)
		{
			--_index;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		operator--();
		return temp;
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (_index >= _owner->_size)
		{
			throw std::runtime_error("Invalid Index!");
		}
		return *(_owner->_data + _index);
	}

	template <typename T>
	const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_index >= _owner->_size)
		{
			throw std::runtime_error("Invalid Index!");
		}
		return *(_owner->_data + _index);
	}

	template <typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept :
		_size(rhs._size), _capacity(rhs._capacity), _data(rhs._data)
	{
		rhs._size = 0;
		rhs._capacity = 0;
		rhs._data = nullptr;
	}

	template <typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free(_data);

			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;

			rhs._size = 0;
			rhs._capacity = 0;
			rhs._data = nullptr;
		}

		return *this;
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		Iterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		ConstIterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator(*this, 0);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0);
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, _size);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator(*this, _size);
	}

	template <typename T>
	typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, _size);
	}

	template <typename T>
	bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& it)
	{
		if (it._owner != this)
		{
			throw std::invalid_argument("Invalid iterator. Owned by different container.");
		}

		bool found = false;
		if (it._index < _size)
		{
			_data[it._index].~T();

			auto size = (_size - it._index - 1) * sizeof(T);
			if (size > 0)
			{
#if defined(_WINDOWS)
				memmove_s(&_data[it._index], size, &_data[it._index + 1], size);
#else
				memmove(&_data[it._index], &_data[it._index + 1], size);
#endif
			}

			--_size;
			found = true;
		}

		return found;
	}

	template <typename T>
	void Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if ((first._owner != last._owner) || first._owner != this)
		{
			throw std::invalid_argument("Invalid iterator. Owned by different container.");
		}

		if (last._index < first._index)
		{
			throw std::runtime_error("Invalid iterator order");
		}

		const auto elementsToRemove = last._index - first._index;
		if (elementsToRemove > 0)
		{
			for (size_t i = first._index; i < last._index; ++i)
			{
				_data[i].~T();
			}

			const auto elementsToMove = _size - last._index;
			if (elementsToMove > 0)
			{
				const auto size = elementsToMove * sizeof(T);
#if defined(_WINDOWS)
				memmove_s(&_data[first._index], size, &_data[last._index], size);
#else
				memmove(&_data[first._index], &_data[last._index], size);
#endif
			}
		_size -= elementsToRemove;
		}
	}

	template <typename T>
	template <typename... Args>
	typename Vector<T>::Iterator Vector<T>::EmplaceBack(Args&&...args)
	{
		if (_size == _capacity)
		{
			std::size_t capacity = _capacity + std::max(size_t(1), _IncrementFunctor(_size, _capacity));
			Reserve(capacity);
		}

		new (_data + _size)T(std::forward<Args>(args)...);
		return Iterator(*this, _size++);
	}

	template <typename T>
	void Vector<T>::Resize(std::size_t size)
	{
		if (size < _size)
		{
			for (std::size_t i = size; i < _size; ++i)
			{
				_data[i].~T();
			}
		}
		else
		{
			Reserve(size);

			for (std::size_t i = _size; i < size; ++i)
			{
				new(_data + i)T();
			}
		}

		_size = size;
	}

	template <typename T>
	void Vector<T>::Resize(std::size_t size, const T& prototype)
	{
		if (size < _size)
		{
			for (std::size_t i = size; i < _size; ++i)
			{
				_data[i].~T();
			}
		}
		else
		{
			Reserve(size);

			for (std::size_t i = _size; i < size; ++i)
			{
				new(_data + i)T(prototype);
			}
		}

		_size = size;
	}

	template<typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (_capacity > _size)
		{
			T* data = reinterpret_cast<T*>(realloc(_data, sizeof(T) * _size));
			_data = data;
			_capacity = _size;
		}
	}
}