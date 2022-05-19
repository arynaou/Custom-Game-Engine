#include "SList.h"

namespace FieaGameEngine
{
	template <typename T>
	template <typename... Args>
	inline SList<T>::Node::Node(Node* next, Args&&...args) :
		Data(std::forward<Args>(args)...), Next(next)
	{
	}

	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		Data(data), Next(next)
	{}

	template <typename T>
	SList<T>::SList(const SList& other)
	{
		Node* currentNode = other._front;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->Data);
			currentNode = currentNode->Next;
		}
	}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList<T>& other)
	{
		if (this != &other)
		{
			Clear();

			Node* currentNode = other._front;
			while (currentNode != nullptr)
			{
				PushBack(currentNode->Data);
				currentNode = currentNode->Next;
			}
		}
		return *this;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (_size==0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}

	template <typename T>
	inline const std::size_t& SList<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		_front = new Node(value, _front);
		Iterator it(*this, _front);

		if (_size ==0)
		{
			_back = _front;
		}
		_size++;
		return it;
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (_front == nullptr) return;

		Node* deleteNode = _front;
		_front = _front->Next;
		delete deleteNode;

		_size--;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{
		_back = new Node(value);
		_size++;
		Iterator it(*this, _back);

		if (_front == nullptr)
		{
			_front = _back;
			return it;
		}

		Node* currentNode = _front;
		while (currentNode->Next != nullptr)
		{
			currentNode = currentNode->Next;
		}

		currentNode->Next = _back;
		return it;
	}

	template <typename T>
	void SList<T>::PopBack()
	{
		if (_front == nullptr) return;
		
		Node* deleteNode = _front;

		if (_size == 1)
		{
			_front = nullptr;
			_back = nullptr;
		}
		else if (_size == 2)
		{
			deleteNode = _front->Next;
			_back = _front;
		}
		else 
		{
			Node* newLastNode = _front;
			while (deleteNode->Next != nullptr)
			{
				newLastNode = deleteNode;
				deleteNode = deleteNode->Next;
			}
			newLastNode->Next = nullptr;
			_back = newLastNode;
		}
		delete deleteNode;
		_size--;
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		Node* currentNode = _front;
		while (currentNode != nullptr)
		{
			Node* nextNode = currentNode->Next;
			delete currentNode;
			currentNode = nextNode;
		}
		_front = _back = nullptr;
		_size = 0;
	}

	template <typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner,const Node* node) :
		_owner(&owner), _node(node)
	{
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		return (_node != other._node);
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return (_node == other._node);
	}


	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("You cannot dereference a nullptr!");
		}
		return _node->Data;
	}

	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator & other):
		_node(other._node), _owner(other._owner)
	{
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (_node != other._node);
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return (_node == other._node);
	}


	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template <typename T>
	const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("You cannot dereference a nullptr!");
		}
		return _node->Data;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value)
	{
		Iterator it;
		for (it = begin(); it !=end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}

		return it;
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
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
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, _front);
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, _front);
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, _front);
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this);
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this);
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& insertPointer, const T& value)
	{
		if (insertPointer._owner != this)
		{
			throw std::runtime_error("Not the same SList!");
		}

		_size++;
		insertPointer._node->Next = new Node(value, insertPointer._node->Next);

		return Iterator(*this,insertPointer._node->Next);
	}

	template <typename T>
	void SList<T>::Remove(T& value)
	{

		Iterator it;
		
		if (_back->Data == value)
		{
			PopBack();
			return;
		}

		Node* currentNode = _front;
		Node* prevNode = nullptr;
		
		for (it = begin(); it != end(); ++it)
		{
			prevNode = currentNode;
			currentNode = currentNode->Next;
			if (*it == value)
			{
				_size--;
				prevNode->Next = currentNode->Next;
				delete currentNode;
				break;
			}
		}
	}

	template <typename T>
	bool SList<T>::Remove(Iterator position)
	{
		if (position._owner != this)
		{
			throw std::runtime_error("Iterator is not associated with this container.");
		}

		if (position == end() || position._node == _back)
		{
			PopBack();
		}
		else
		{
			Node* next = position._node->Next;
			position._node->Data.~T();
			new (&position._node->Data)T(std::move(next->Data));
			position._node->Next = next->Next;
			delete next;
			_size--;
		}

		return true;
	}

	template <typename T>
	template <typename... Args>
	typename SList<T>::Iterator SList<T>::EmplaceBack(Args&&... args)
	{
		Node* node = new Node(nullptr, std::forward<Args>(args)...);

		if (_size == 0)
		{
			_front = node;
		}
		else
		{
			_back->Next = node;
		}
		
		_back = node;
		++_size;

		return Iterator(*this, _back);
	}

	template <typename T>
	template <typename... Args>
	typename SList<T>::Iterator SList<T>::EmplaceFront(Args&&... args)
	{
		Node* node = new Node(_front, std::forward<Args>(args)...);

		_front = node;
		Iterator it(*this, _front);

		if (_size == 0)
		{
			_back = _front;
		}
		++_size;
		return it;
	}
}