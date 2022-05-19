#include "pch.h"
#include "Scope.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(Scope* parent, std::size_t capacity)
	{
		_parent = parent;
		_hashMap = HashMap<std::string, Datum>(capacity, DefaultHash<std::string>{});
		_orderVector.Reserve(capacity);
	}

	Scope::~Scope()
	{
		Orphan();
		Clear();
	}

	Scope::Scope(const Scope& other) 
	{
		if (this != &other)
		{
			for (auto pair : other._orderVector)
			{
				auto [key, existingDatum] = *pair;
				Datum& newDatum = Append(key);
				newDatum.SetType(existingDatum.Type());
				if (existingDatum.Type() == DatumTypes::Table)
				{
					newDatum.Reserve(existingDatum.Size());
					for (std::uint32_t index = 0; index < existingDatum.Size(); ++index)
					{
						Scope& nestedScope = existingDatum[index];
						Scope* childCopy = nestedScope.Clone();
						childCopy->_parent = this;
						newDatum.PushBack(childCopy);
					}
				}
				else
				{
					newDatum = existingDatum;
				}
			}
		}
	}

	Scope& Scope::operator = (const Scope& other)
	{
		if (this != &other)
		{
			for (auto pair : other._orderVector)
			{
				auto [key, existingDatum] = *pair;
				Datum& newDatum = Append(key);
				newDatum.SetType(existingDatum.Type());
				if (existingDatum.Type() == DatumTypes::Table)
				{
					newDatum.Reserve(existingDatum.Size());
					for (std::uint32_t index = 0; index < existingDatum.Size(); ++index)
					{
						Scope& nestedScope = existingDatum[index];
						Scope* childCopy = nestedScope.Clone();
						childCopy->_parent = this;
						newDatum.PushBack(childCopy);
					}
				}
				else
				{
					newDatum = existingDatum;
				}
			}
		}
		return *this;
	}

	Scope::Scope(Scope&& other) noexcept
	{
		_parent = other._parent;
		_hashMap = std::move(other._hashMap);
		_orderVector = std::move(other._orderVector);
		if (other._parent != nullptr)
		{
			auto [datump, index] = other._parent->FindContainedScope(&other);
			if (datump != nullptr) datump->Get<Scope*>(index) = this;
		}

		for (std::size_t i = 0; i < _orderVector.Size(); ++i)
		{
			Datum& datum = _orderVector[i]->second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t j = 0; j < datum.Size(); ++j)
				{
					datum[j]._parent = this;
				}
			}
		}

		other._parent = nullptr;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		Clear();

		_parent = other._parent;
		_hashMap = std::move(other._hashMap);
		_orderVector = std::move(other._orderVector);
		if (other._parent != nullptr)
		{
			auto [datump, index] = other._parent->FindContainedScope(&other);
			if (datump != nullptr) datump->Get<Scope*>(index) = this;
		}

		for (std::size_t i = 0; i < _orderVector.Size(); ++i)
		{
			Datum& datum = _orderVector[i]->second;
			if (datum.Type() == DatumTypes::Table)
			{
				for (std::size_t j = 0; j < datum.Size(); ++j)
				{
					datum[j]._parent = this;
				}
			}
		}

		other._parent = nullptr;

		return *this;
	}

	Datum* Scope::Find(const std::string& key)
	{
		if (_hashMap.Find(key) == _hashMap.end())
		{
			return nullptr;
		}
		else
		{
			return &(_hashMap.At(key));
		}
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		if (_hashMap.Find(key) == _hashMap.end())
		{
			return nullptr;
		}
		else
		{
			return &(_hashMap.At(key));
		}
	}

	Datum* Scope::Search(const std::string& key)
	{
		Scope* scope;
		return Search(key, scope);
	}

	const Datum* Scope::Search(const std::string& key) const
	{
		const Scope* scope;
		return Search(key, scope);
	}

	Datum* Scope::Search(const std::string& key, Scope*& foundScope)
	{
		foundScope = nullptr;

		Datum* find = Find(key);
		if (find != nullptr)
		{
			foundScope = this;
		}
		else if (_parent != nullptr)
		{
			find = _parent->Search(key, foundScope);
		}
		return find;
	}

	const Datum* Scope::Search(const std::string& key, const Scope*& foundScope) const
	{
		foundScope = nullptr;

		const Datum* find = Find(key);
		if (find != nullptr)
		{
			foundScope = this;
		}
		else if (_parent != nullptr)
		{
			find = _parent->Search(key, foundScope);
		}
		return find;
	}

	Datum& Scope::Append(const std::string& key)
	{
		if (key.empty())
		{
			throw std::runtime_error("The append key shouldn't be empty!");
		}

		auto [iterator, isInserted] = _hashMap.Insert(std::make_pair(key, Datum{}));
		if (isInserted)
		{
			_orderVector.EmplaceBack(&(*iterator));
		}
		return (*iterator).second;
	}

	Scope& Scope::AppendScope(const std::string& key)
	{
		Datum* find = Find(key);
		if (find != nullptr)
		{
			if (find->Type() == DatumTypes::Unknown)
			{
				find->SetType(DatumTypes::Table);
			}

			if (find->Type() == DatumTypes::Table)
			{
				Scope* scope = new Scope();
				find->PushBack(scope);
				scope->SetParent(this);
				return *scope;
			}
			else
			{
				throw std::runtime_error("The key is already exist and the type is not table!");
			}
		}
		else
		{
			Datum& datum = Append(key);
			Scope* scope = new Scope();
			datum.PushBack(scope);
			scope->SetParent(this);
			return *scope;
		}
	}

	void Scope::Adopt(Scope& scope, const std::string& key)
	{
		scope.Orphan();

		scope.SetParent(this);
		HashMap<std::string, Datum>::PairType pair = { key, &scope };
		pair.second.SetType(DatumTypes::Table);
		auto [iterator, isInserted] = _hashMap.Insert(pair);
		if (isInserted)
		{
			_orderVector.EmplaceBack(&(*iterator));
		}
		else
		{
			(*iterator).second.PushBack(&scope);
		}
	}

	Scope* Scope::GetParent()
	{
		return _parent;
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	template <typename T>
	T& Scope::GetParent()
	{
		assert(_parent != nullptr);
		assert(_parent->Is(T::TypeIdClass()));
		return static_cast<T&>(*_parent);
	}

	void Scope::SetParent(Scope* parent)
	{
		Orphan();
		_parent = parent;
	}

	Datum& Scope::operator[](const std::string& string)
	{
		return Append(string);
	}

	Datum& Scope::operator[](std::size_t index)
	{
		return _orderVector[index]->second;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		for (std::size_t i = 0; i < _orderVector.Size(); ++i)
		{
			Datum datum = _hashMap.At(_orderVector[i]->first);
			const Datum* rhsDatum = rhs.Find(_orderVector[i]->first);

			if (rhs.Find(_orderVector[i]->first) == nullptr) return false;

			if (datum.Type() != rhsDatum->Type()) return false;

			if (datum.Type() != DatumTypes::Table)
			{
				if (datum != *rhsDatum) return false;
			}
			else
			{
				if (datum.Get<Scope*>(i) != rhsDatum->Get<Scope*>(i)) return false;
			}
		}
		return true;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(const Scope* scope)
	{
		for (std::size_t i = 0; i < _orderVector.Size(); ++i)
		{
			if (_orderVector[i]->second.Type() == DatumTypes::Table)
			{
				Datum& datum = _orderVector[i]->second;
				size_t index = datum.Find(scope);
				if (index != datum.Size())
				{
					return std::make_pair(&datum, index);
				}
			}
		}
		return std::make_pair(nullptr, _orderVector.Size());
	}

	void Scope::Orphan()
	{
		if (_parent != nullptr)
		{
			auto [datum, i] =_parent->FindContainedScope(this);
			if (datum != nullptr)
			{
				(*datum).RemoveAt(i);
			}
		}

		_parent = nullptr;
	}

	void Scope::Clear()
	{
		for (std::size_t i = 0; i < _orderVector.Size(); ++i)
		{
			if (_orderVector[i]->second.Type() == DatumTypes::Table)
			{
				for (std::size_t j = 0; j < _orderVector[i]->second.Size(); ++j)
				{
					Scope* scope = _orderVector[i]->second.Get<Scope*>(j);
					scope->_parent = nullptr;
					delete scope;
				}
			}
		}
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs != nullptr)
		{
			if (rhs->Is(Scope::TypeIdClass()))
			{
				const Scope* other = reinterpret_cast<const Scope*>(rhs);
				return (*this == *other);
			}
		}
		return false;
	}

	Datum& Scope::At(const std::string& string)
	{
		Datum* datum = Find(string);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}

	const Datum& Scope::At(const std::string& string) const
	{
		const Datum* datum = Find(string);
		if (datum == nullptr)
		{
			throw std::runtime_error("Expected entry.");
		}

		return *datum;
	}

	const size_t Scope::Size()
	{
		return _orderVector.Size();
	}
}