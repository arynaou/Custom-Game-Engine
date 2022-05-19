#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(std::size_t typeID)
	{
		Populate(typeID);
	}

	Attributed::Attributed(const Attributed& other) : Scope(other)
	{
		_orderVector[0]->second.Set(this);

		auto& signature = TypeManager::GetSignature(other.TypeIdInstance());
		for (std::size_t i = 1; i <= signature.Size(); ++i)
		{
			Datum& datum = _orderVector[i]->second;
			if (!datum.IsInternal())
			{
				uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
				datum.SetStorage(thisptr + signature[i-1]._offset, signature[i-1]._size, signature[i-1]._type);
			}
		}
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{

			Scope::operator=(other);
			_orderVector[0]->second.Set(this);
			auto& signature = TypeManager::GetSignature(other.TypeIdInstance());
			for (std::size_t i = 1; i <= signature.Size(); ++i)
			{
				Datum& datum = _orderVector[i]->second;
				if (!datum.IsInternal())
				{
					uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
					datum.SetStorage(thisptr + signature[i - 1]._offset, signature[i - 1]._size, signature[i - 1]._type);
				}
			}
		}

		return *this;
	}

	Attributed::Attributed(Attributed&& other) noexcept
		: Scope(std::move(other))
	{
		_orderVector[0]->second.Set(this);
		auto& signature = TypeManager::GetSignature(other.TypeIdInstance());
		for (std::size_t i = 1; i <= signature.Size(); ++i)
		{
			Datum& datum = _orderVector[i]->second;
			if (!datum.IsInternal())
			{
				uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
				datum.SetStorage(thisptr + signature[i - 1]._offset, signature[i - 1]._size, signature[i - 1]._type);
			}
		}
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			_orderVector[0]->second.Set(this);
			auto& signature = TypeManager::GetSignature(other.TypeIdInstance());
			for (std::size_t i = 1; i <= signature.Size(); ++i)
			{
				Datum& datum = _orderVector[i]->second;
				if (!datum.IsInternal())
				{
					uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
					datum.SetStorage(thisptr + signature[i - 1]._offset, signature[i - 1]._size, signature[i - 1]._type);
				}
			}
		}
		return *this;
	}

	bool Attributed::IsAttribute(const std::string& value)
	{
		return (_hashMap.Find(value) != _hashMap.end());
	}

	bool Attributed::IsPrescribedAttribute(const std::string& value)
	{
		if (std::strcmp(value.c_str(), "this")==0)
		{
			return true;
		}

		const Vector<Signature>& signature = TypeManager::GetSignature(TypeIdInstance());
		for (std::size_t i = 0; i < signature.Size(); ++i)
		{
			if (signature[i]._name == value)
			{
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& value)
	{
		return (IsAttribute(value) && !IsPrescribedAttribute(value));
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& value)
	{
		if (IsPrescribedAttribute(value))
		{
			throw std::runtime_error("the key is prescribed, you can't change it to auxiliary!");
		}

		return Append(value);
	}

	const Vector<Signature> Attributed::Signatures()
	{
		return Vector<Signature>{};
	}

	void Attributed::Populate(std::size_t typeID)
	{
		(*this)["this"s] = this;
		const Vector<Signature>& signature = TypeManager::GetSignature(typeID);
		for (std::size_t i = 0; i < signature.Size(); ++i)
		{
			Datum& datum = this->Append(signature[i]._name);
			datum.SetType(signature[i]._type);
			if (signature[i]._type != DatumTypes::Table)
			{
				uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
				datum.SetStorage(thisptr + signature[i]._offset, signature[i]._size, signature[i]._type);
			}
		}
	}
}
