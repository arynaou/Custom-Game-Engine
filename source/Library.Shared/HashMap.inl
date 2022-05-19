#include "HashMap.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(const size_t size, const HashFunctor hashFunctor)
	{
		_bucket.Resize(size);
		_hashFunctor = hashFunctor;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key)
	{
		std::size_t index = _hashFunctor(key) % _bucket.Size();

		for (typename ChainType::Iterator chainIterator = _bucket[index].begin(); chainIterator!= _bucket[index].end();++chainIterator)
		{
			if ((*chainIterator).first == key)
			{
				return Iterator{ *this, index,chainIterator };
			}
		}

		return end();

	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key) const
	{
		std::size_t index = _hashFunctor(key) % _bucket.Size();

		for (typename ChainType::ConstIterator chainIterator = _bucket[index].begin(); chainIterator != _bucket[index].end(); ++chainIterator)
		{
			if ((*chainIterator).first == key)
			{
				return ConstIterator{ *this, index,chainIterator };
			}
		}

		return cend();
	}

	template <typename TKey, typename TData>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& pair)
	{
		return Emplace(pair);
	}

	template <typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		TData defaultData = 0;
		if (Find(key) == end())
		{
			Insert({ key , defaultData });
		}

		return (*Find(key)).second;
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::Remove(const TKey& key)
	{
		auto position = Find(key);
		if (position != end())
		{
			_bucket[position._index].Remove(position._chainIterator);
			--_size;
		}
	}

	template <typename TKey, typename TData>
	void HashMap<TKey, TData>::Clear()
	{
		for (size_t i = 0; i < _bucket.Size(); ++i)
		{
			_bucket[i].Clear();
		}
		_size = 0;
	}

	template <typename TKey, typename TData>
	const std::size_t& HashMap<TKey, TData>::Size() const
	{
		return _size;
	}

	template <typename TKey, typename TData>
	const bool HashMap<TKey, TData>::ContainsKey(const TKey& key)
	{
		return !(Find(key) == end());
	}

	template <typename TKey, typename TData>
	const bool HashMap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return !(Find(key) == end());
	}

	template <typename TKey, typename TData>
	TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		auto position = Find(key);
		if (position == end())
		{
			throw std::runtime_error("Could not find key.");
		}

		return (*position).second;
	}

	template <typename TKey, typename TData>
	const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		auto position = Find(key);
		if (position == end())
		{
			throw std::runtime_error("Could not find key.");
		}

		return (*position).second;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		if (_size > 0)
		{
			std::size_t index = 0;
			for (; index < _bucket.Size(); ++index)
			{

				if (_bucket[index].Size() != 0)
				{
					break;
				}
			}
			return Iterator(*this, index, _bucket[index].begin());
		}
		else
		{
			return end();
		}
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		if (_size > 0)
		{
			std::size_t index = 0;
			for (; index < _bucket.Size(); ++index)
			{

				if (_bucket[index].Size() != 0)
				{
					break;
				}
			}
			return ConstIterator(*this, index, _bucket[index].cbegin());
		}
		else
		{
			return cend();
		}
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		if (_size > 0)
		{
			std::size_t index = 0;
			for (; index < _bucket.Size(); ++index)
			{
				if (_bucket[index].Size() != 0)
				{
					break;
				}
			}
			return ConstIterator(*this, index, _bucket[index].cbegin());
		}
		else
		{
			return cend();
		}
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator(*this, _bucket.Size() - 1, _bucket[_bucket.Size() - 1].end());
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, _bucket.Size() - 1, _bucket[_bucket.Size() - 1].cend());
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		return ConstIterator(*this, _bucket.Size() - 1, _bucket[_bucket.Size() - 1].cend());
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::Iterator::Iterator(HashMap& owner, size_t index, typename ChainType::Iterator chainIterator) :
		_owner(&owner), _index(index), _chainIterator(chainIterator)
	{
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::ConstIterator::ConstIterator(const HashMap& owner, size_t index, typename ChainType::ConstIterator chainIterator) :
		_owner(&owner), _index(index), _chainIterator(chainIterator)
	{
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_index = other._index;
		_chainIterator = other._chainIterator;

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator=(const Iterator& other)
	{
		_owner = other._owner;
		_index = other._index;
		_chainIterator = other._chainIterator;

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator=(const ConstIterator& other)
	{
		_owner = other._owner;
		_index = other._index;
		_chainIterator = other._chainIterator;

		return *this;
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& other) const
	{
		return !(operator==(other));
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& other) const
	{
		return (_owner == other._owner && _index == other._index && _chainIterator == other._chainIterator) || (_owner == other._owner && _index == _owner->_bucket.Size()&& _index == other._index);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return !(operator==(other));
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return _owner == other._owner && _index == other._index && _chainIterator == other._chainIterator || (_owner == other._owner && _index == _owner->_bucket.Size()&& _index == other._index);
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_index < _owner->_bucket.Size())
		{

			++_chainIterator;
			if (_chainIterator != _owner->_bucket[_index].end())
			{
				while (_index < _owner->_bucket.Size() && _owner->_bucket[_index].IsEmpty())
				{
					++_index;
				}
				if (_index < _owner->_bucket.Size())
				{
					_chainIterator = _owner->_bucket[_index].begin();
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container!");
		}

		if (_index < _owner->_bucket.Size())
		{

			++_chainIterator;
			if (_chainIterator != _owner->_bucket[_index].cend())
			{
				while (_index < _owner->_bucket.Size() && _owner->_bucket[_index].IsEmpty())
				{
					++_index;
				}
				if (_index < _owner->_bucket.Size())
				{
					_chainIterator = _owner->_bucket[_index].cbegin();
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey,TData>::PairType& HashMap<TKey, TData>::Iterator::operator*() const
	{		 
		return *_chainIterator;
	}

	template <typename TKey, typename TData>
	typename const HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::ConstIterator::operator*() const
	{
		return *_chainIterator;
	}

	template <typename TKey, typename TData>
	template <typename...Args>
	std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Emplace(Args&&... args)
	{
		PairType item(std::forward<Args>(args)...);
		std::size_t index;
		bool itemInserted;
		Iterator it = Find(item.first,index);
		if (it == end())
		{
			auto chainIterator = _bucket[index].EmplaceBack(std::move(item));
			++_size;

			it = Iterator(*this, index, chainIterator);
			itemInserted = true;
		}
		else
		{
			itemInserted = false;
		}

		return std::make_pair(std::move(it), itemInserted);
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>& HashMap<TKey, TData>::operator=(std::initializer_list<PairType> list)
	{
		Clear();
		_bucket.Resize(list.size());
		for (const auto& pair : list)
		{
			Emplace(pair);
		}
		return *this;
	}

	template <typename TKey, typename TData>
	typename HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, std::size_t& index)
	{
		index = _hashFunctor(key) % _bucket.Size();
		ChainType& chain = _bucket[index];
		for (auto chainIterator = chain.begin(); chainIterator != chain.end(); ++chainIterator)
		{
			PairType& pair = *chainIterator;
			if (pair.first == key)
			{
				return Iterator(*this, index, chainIterator);
			}
		}

		return end();
	}

	template <typename TKey, typename TData>
	HashMap<TKey, TData>::HashMap(std::initializer_list<PairType> list, const HashFunctor hashFunctor)
	{
		_hashFunctor = hashFunctor;
		_bucket.Resize(list.size());

		for (const auto& pair : list)
		{
			Emplace(pair);
		}
	}
}
