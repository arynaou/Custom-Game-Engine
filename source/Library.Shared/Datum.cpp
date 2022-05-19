#include "pch.h"
#include "Datum.h"

namespace FieaGameEngine
{
	const std::size_t Datum::DatumTypeSizes[] =
	{
		0, //UNKNOWN
		sizeof(std::int32_t),
		sizeof(float),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(std::string),
		sizeof(RTTI*),
		sizeof(Scope*)
	};

	Datum::Datum(DatumTypes type, std::size_t capacity) :
		_type(type)
	{
		if (capacity > 0)
		{
			if (type == DatumTypes::Unknown)
			{
				throw std::runtime_error("Cannot set an unknown type");
			}
			Reserve(capacity);
		}
	}

	Datum::~Datum()
	{
		if (_isInternal == false) return;

		Clear();
		free(_data.vp);
	}

	Datum::Datum(const Datum& other) :
		_type(other._type), _size(other._size), _isInternal{ other._isInternal }
	{
		if (_isInternal == false)
		{
			SetStorage(other._data.vp, _size, _type);
			return;
		}
		if (other._type == DatumTypes::Unknown) return;
		Reserve(other._capacity);
		if (_type == DatumTypes::String)
		{
			for (std::size_t i = 0; i < _size; ++i)
			{
				new (_data.s + i)std::string(*(other._data.s + i));
			}
		}
		else
		{
			memcpy(_data.vp, other._data.vp, _size * DatumTypeSizes[static_cast<std::size_t>(_type)]);
		}
	}

	Datum::Datum(const std::int32_t& other)
	{
		_size = 0;
		_type = DatumTypes::Integer;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(const float& other)
	{
		_size = 0;
		_type = DatumTypes::Float;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(const glm::vec4& other)
	{
		_size = 0;
		_type = DatumTypes::Vectors;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(const glm::mat4& other)
	{
		_size = 0;
		_type = DatumTypes::Matrix;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(const std::string other)
	{
		_size = 0;
		_type = DatumTypes::String;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(RTTI* other)
	{
		_size = 0;
		_type = DatumTypes::Pointer;
		Reserve(1);
		PushBack(other);
	}

	Datum::Datum(Scope* other)
	{
		_size = 0;
		_type = DatumTypes::Table;
		Reserve(1);
		PushBack(other);
	}

	Datum& Datum::operator=(const Datum& other)
	{
		_type = other._type;

		if (!other._isInternal)
		{
			SetStorage(other._data.vp, _size, _type);
		}
		else
		{
			if (_isInternal)
			{
				Clear();
				ShrinkToFit();
				Reserve(other._size);
				_size = other._size;
			}

			if (_type == DatumTypes::String)
			{
				for (std::size_t i = 0; i < _size; ++i)
				{
					new (_data.s + i)std::string(*(other._data.s + i));
				}
			}
			else
			{
				memcpy(_data.vp, other._data.vp, _size * DatumTypeSizes[static_cast<std::size_t>(_type)]);
			}
		}
		return *this;
	}

	Datum& Datum::operator=(const std::int32_t& other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::Integer;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	Datum& Datum::operator=(const float& other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::Float;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::Vectors;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::Matrix;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	Datum& Datum::operator=(const std::string& other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::String;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	Datum& Datum::operator=(RTTI* other)
	{
		if (!_isInternal) throw std::runtime_error("Cannot change the memory");
		Clear();

		_type = DatumTypes::Pointer;
		Reserve(1);
		PushBack(other);

		return *this;
	}

	void Datum::Reserve(std::size_t capacity)
	{
		if (capacity > _capacity)
		{
			if (_type == DatumTypes::Unknown)
			{
				throw std::runtime_error("Invalid operation");
			}

			if (!_isInternal) throw std::runtime_error("Cannot change its Capactiy");


			void* data = realloc(_data.vp, capacity * DatumTypeSizes[static_cast<std::size_t>(_type)]);
			assert(data);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Resize(std::size_t capacity)
	{
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Invalid operation. Type must be set.");
		}

		if (!_isInternal) throw std::runtime_error("Cannot change the size");

		if (_size < capacity)
		{
			Reserve(capacity);
			if (_type == DatumTypes::String)
			{
				for (std::size_t i = _size; i < capacity; ++i)
				{
					new (_data.s + i)std::string();
				}
			}
			_size = capacity;
		}
		else if(_size> capacity)
		{
			while (_size > capacity)
			{
				PopBack();
			}
			_capacity = capacity;
		}
	}

	const DatumTypes Datum::Type() const
	{
		return _type;
	}

	std::size_t Datum::Size() const
	{
		return _size;
	}

	std::size_t Datum::Capacity() const
	{
		return _capacity;
	}

	template<>
	std::int32_t& Datum::Front<std::int32_t>()
	{
		if (_size == 0) throw std::runtime_error("Cannot access an empty Datum.");

		return *(_data.i);
	}

	template<>
	float& Datum::Front<float>()
	{
		if (_size == 0) throw std::runtime_error("Cannot access an empty Datum.");


		return *(_data.f);
	}

	template<>
	glm::vec4& Datum::Front<glm::vec4>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.v);
	}

	template<>
	glm::mat4& Datum::Front<glm::mat4>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.m);
	}

	template<>
	std::string& Datum::Front<std::string>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.s);
	}

	template<>
	RTTI*& Datum::Front<RTTI*>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.r);
	}

	template<>
	Scope*& Datum::Front<Scope*>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.sc);
	}

	template<>
	const std::int32_t& Datum::Front<std::int32_t>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.i);
	}

	template<>
	const float& Datum::Front<float>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.f);
	}

	template<>
	const glm::vec4& Datum::Front<glm::vec4>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.v);
	}

	template<>
	const glm::mat4& Datum::Front<glm::mat4>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.m);
	}

	template<>
	const std::string& Datum::Front<std::string>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.s);
	}

	template<>
	RTTI* const & Datum::Front<RTTI*>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.r);
	}

	template<>
	Scope* const& Datum::Front<Scope*>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.sc);
	}

	template<>
	std::int32_t& Datum::Back<std::int32_t>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.i + _size - 1);
	}

	template<>
	float& Datum::Back<float>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.f + _size - 1);
	}

	template<>
	glm::vec4& Datum::Back<glm::vec4>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.v + _size - 1);
	}

	template<>
	glm::mat4& Datum::Back<glm::mat4>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.m + _size - 1);
	}

	template<>
	std::string& Datum::Back<std::string>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.s + _size - 1);
	}

	template<>
	RTTI*& Datum::Back<RTTI*>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.r + _size - 1);
	}

	template<>
	Scope*& Datum::Back<Scope*>()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.sc + _size - 1);
	}

	template<>
	const std::int32_t& Datum::Back<std::int32_t>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.i + _size - 1);
	}

	template<>
	const float& Datum::Back<float>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.f + _size - 1);
	}

	template<>
	const glm::vec4& Datum::Back<glm::vec4>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.v + _size - 1);
	}

	template<>
	const glm::mat4& Datum::Back<glm::mat4>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.m + _size - 1);
	}

	template<>
	const std::string& Datum::Back<std::string>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.s + _size - 1);
	}

	template<>
	RTTI* const & Datum::Back<RTTI*>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.r + _size - 1);
	}

	template<>
	Scope* const& Datum::Back<Scope*>() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty Datum.");
		}

		return *(_data.sc + _size - 1);
	}

	template<>
	std::int32_t& Datum::Get<std::int32_t>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.i + index);
	}

	template<>
	const std::int32_t& Datum::Get<std::int32_t>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.i + index);
	}

	template<>
	float& Datum::Get<float>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.f + index);
	}

	template<>
	const float& Datum::Get<float>(std::size_t index) const 
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.f + index);
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.v + index);
	}

	template<>
	const glm::vec4& Datum::Get<glm::vec4>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.v + index);
	}

	template<>
	glm::mat4& Datum::Get<glm::mat4>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.m + index);
	}

	template<>
	const glm::mat4& Datum::Get<glm::mat4>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.m + index);
	}

	template<>
	std::string& Datum::Get<std::string>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.s + index);
	}

	template<>
	const std::string& Datum::Get<std::string>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.s + index);
	}
	
	template<>
	RTTI*& Datum::Get<RTTI*>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.r + index);
	}

	template<>
	RTTI* const& Datum::Get<RTTI*>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.r + index);
	}

	template<>
	Scope*& Datum::Get<Scope*>(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.sc + index);
	}

	template<>
	Scope* const& Datum::Get<Scope*>(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::runtime_error("Invalid Index.");
		}

		return *(_data.sc + index);
	}


	template<>
	std::string Datum::ToString<std::int32_t>(std::size_t index)
	{
		return std::to_string(*(_data.i + index));
	}

	template<>
	std::string Datum::ToString<float>(std::size_t index)
	{
		return std::to_string(*(_data.f + index));
	}

	template<>
	std::string Datum::ToString<glm::vec4>(std::size_t index)
	{
		std::string s="vec4(";
		for (std::size_t i = 0; i < 4; ++i)
		{
			s += std::to_string((_data.f + index)[i]) + ",";
		}
		s.pop_back();
		s += ")";

		return s;
	}

	template<>
	std::string Datum::ToString<glm::mat4>(std::size_t index)
	{
		std::string s= "mat4x4(";
		for (std::size_t i = 0; i < 16; ++i)
		{
			s += std::to_string((_data.f + index)[i]) + ",";
		}
		s.pop_back();
		s += ")";

		return s;
	}

	template<>
	std::string Datum::ToString<std::string>(std::size_t index)
	{
		return *(_data.s + index);
	}

	template<>
	std::string Datum::ToString<RTTI>(std::size_t index)
	{
		return _data.r[index]->ToString();
	}

	void Datum::SetType(DatumTypes datumType)
	{
		if (_type == datumType) return;
		if (datumType == DatumTypes::Unknown)
		{
			throw std::runtime_error("You can't set type to unknonw");
		}

		if (_type == DatumTypes::Unknown)
		{
			_type = datumType;
			std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
			void* data = realloc(_data.vp, 1 * size);
			_data.vp = data;
			_capacity = 1;
		}
		else
		{
			throw std::runtime_error("The type already be set!");
		}
	}

	void Datum::Set(const std::int32_t& value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Integer)
		{
			if (index > _size-1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Integer);
			*(_data.i + index) = value;
		}
	}


	void Datum::Set(const float& value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Float)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Float);
			*(_data.f + index) = value;
		}
	}

	void Datum::Set(const glm::vec4& value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Vectors)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Vectors);
			*(_data.v + index) = value;
		}
	}

	void Datum::Set(const glm::mat4& value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Matrix)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Matrix);
			*(_data.m + index) = value;
		}
	}

	void Datum::Set(const std::string& value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::String)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::String);
			*(_data.s + index) = value;
		}
	}

	void Datum::Set(RTTI* value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Pointer)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Pointer);
			*(_data.r + index) = value;
		}
	}

	void Datum::Set(Scope* value, std::size_t index)
	{
		if (_type == DatumTypes::Unknown || _type == DatumTypes::Table)
		{
			if (index > _size - 1) throw std::runtime_error("the index shouldn't bigger than the size");

			SetType(DatumTypes::Table);
			*(_data.sc + index) = value;
		}
	}

	void Datum::PushBack(const std::int32_t& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Integer;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");
		
		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.i + _size) = value;
		++_size;
	}

	void Datum::PushBack(const float& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Float;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.f + _size) = value;
		++_size;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Vectors;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.v + _size) = value;
		++_size;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Matrix;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.m + _size) = value;
		++_size;
	}

	void Datum::PushBack(const std::string& value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::String;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		new (_data.s + _size)std::string(value);
		++_size;
	}

	void Datum::PushBack(RTTI* value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Pointer;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.r + _size) = value;
		++_size;
	}

	//TODO reference?? cuz it can't be null, Scope&
	void Datum::PushBack(Scope* value)
	{
		if (_type == DatumTypes::Unknown)
		{
			_type = DatumTypes::Table;
		}

		if (!_isInternal) throw std::runtime_error("You can't change it cause it's external!");

		if (_capacity <= _size)
		{
			Reserve(std::max<std::size_t>(1, _capacity * 2));
		}

		*(_data.sc + _size) = value;
		++_size;
	}

	bool Datum::Remove(const std::int32_t& value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.i + i, _data.i + i + 1, (_size - i) * sizeof(std::int32_t));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const float& value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.f + i, _data.f + i + 1, (_size - i) * sizeof(float));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.v + i, _data.v + i + 1, (_size - i) * sizeof(glm::vec4));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.m + i, _data.m + i + 1, (_size - i) * sizeof(glm::mat4));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const std::string& value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			_data.s[i].~basic_string();
			memmove(_data.s+i, _data.s+i + 1, (_size-i)*sizeof(std::string));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const RTTI* value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.r + i, _data.r + i + 1, (_size - i) * sizeof(RTTI*));
			--_size;
			found = true;
		}

		return found;
	}

	bool Datum::Remove(const Scope* value)
	{
		bool found = false;
		size_t i = Find(value);
		if (i < _size && _size>0)
		{
			memmove(_data.sc + i, _data.sc + i + 1, (_size - i) * sizeof(Scope*));
			--_size;
			found = true;
		}

		return found;
	}

	std::size_t Datum::Find(const std::int32_t& value) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.i + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const float& value) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.f + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const glm::vec4& value) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.v + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const glm::mat4& value) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.m + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const std::string& value) const
	{
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.s + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const RTTI* value) const
	{
		
		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.r + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	std::size_t Datum::Find(const Scope* value) const
	{

		for (std::size_t i = 0; i < _size; ++i)
		{
			if (*(_data.sc + i) == value)
			{
				return i;
			}
		}

		return _size;
	}

	void Datum::Clear()
	{
		if (!_isInternal) return;

		if (_type == DatumTypes::String)
		{
			for (size_t i = 0; i < _size; ++i)
			{
				_data.s[i].~basic_string();
			}
		}
		_size = 0;
	}

	void Datum::ShrinkToFit()
	{
		if (_type == DatumTypes::Unknown)
		{
			return;
			//throw std::runtime_error("Invalid operation");
		}

		if (!_isInternal) return;

		if (_size == 0)
		{
			free(_data.vp);
			_data.vp = nullptr;
			_capacity = 0;
		}
		else
		{
			_capacity = _size;
			void* data = realloc(_data.vp, _capacity * DatumTypeSizes[static_cast<std::size_t>(_type)]);
			assert(data);
			_data.vp = data;
		}
	}

	bool Datum::operator==(const Datum& other) const
	{
		if (_size == other._size && _type == other._type)
		{ 
			if (_type == DatumTypes::String)
			{
				for (std::size_t i = 0; i < _size; ++i)
				{
					if (*(_data.s + i) != *(other._data.s + i))
					{
						return false;
					}
				}
				return true;
			}
			else if (_type == DatumTypes::Pointer)
			{
				for (std::size_t i = 0; i < _size; ++i)
				{
					if (!_data.r[i]->Equals(other._data.r[i]))
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return (std::memcmp(_data.vp, other._data.vp, _size * DatumTypeSizes[static_cast<std::size_t>(_type)]) ==0);
			}
		}
		return false;
	}

	bool Datum::operator==(const std::int32_t& other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}

		if (_type != DatumTypes::Integer)
		{
			return false;
		}
		
		return (*(_data.i) == other);
	}

	bool Datum::operator==(const float& other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}

		if (_type != DatumTypes::Float)
		{
			return false;
		}
		
		return (*(_data.f) == other);
	}

	bool Datum::operator==(const glm::vec4& other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}

		if (_type != DatumTypes::Vectors)
		{
			return false;
		}

		return (*(_data.v) == other);
	}

	bool Datum::operator==(const glm::mat4& other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}
		
		if (_type != DatumTypes::Matrix)
		{
			return false;
		}

		return (*(_data.m) == other);
	}

	bool Datum::operator==(const std::string& other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}

		if (_type != DatumTypes::String)
		{
			return false;
		}

		return (std::strncmp(_data.s->c_str(), other.c_str(), _data.s->size()) ==0);
	}

	bool Datum::operator==(const RTTI* other) const
	{
		if (_size != 1)
		{
			throw std::runtime_error("you can't compare one element with multiple elements");
		}

		if (_type != DatumTypes::Pointer)
		{
			return false;
		}
		
		return (_data.r[0]->Equals(other));
	}

	//bool Datum::operator==(const Scope* other) const
	//{
	//	if (_size != 1)
	//	{
	//		throw std::runtime_error("you can't compare one element with multiple elements");
	//	}

	//	if (_type != DatumTypes::Table)
	//	{
	//		return false;
	//	}

	//	return (_data.sc[0]->Equals(other));
	//}

	bool Datum::operator!=(const Datum& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const std::int32_t& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const float& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const glm::vec4& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const glm::mat4& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const std::string& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator!=(const RTTI* other) const
	{
		return !(operator==(other));
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free(_data.vp);

			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;
			_type = rhs._type;

			rhs._size = 0;
			rhs._capacity = 0;
			rhs._data.vp = nullptr;
			rhs._type = DatumTypes::Unknown;
		}

		return *this;
	}

	Datum::Datum(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Clear();
			free(_data.vp);

			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;
			_type = rhs._type;

			rhs._size = 0;
			rhs._capacity = 0;
			rhs._data.vp = nullptr;
			rhs._type = DatumTypes::Unknown;
		}
	}

	void Datum::SetFromString(std::string value, std::size_t index)
	{
		switch (_type) 
		{
		case DatumTypes::Integer:
			Set(std::stoi(value), index);
			break;
		case DatumTypes::Float:
			Set(std::stof(value), index);
			break;
		case DatumTypes::Vectors:
			float v1, v2, v3, v4;
			sscanf_s(value.c_str(), "vec4(%f,%f,%f,%f)", &v1, &v2, &v3, &v4);
			Set(glm::vec4{ v1, v2, v3, v4 }, index);
			break;
		case DatumTypes::Matrix:
			float v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;
			sscanf_s(value.c_str(), "mat4x4((%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f))", &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10, &v11, &v12, &v13, &v14, &v15, &v16);
			Set(glm::mat4(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16), index);
			break;
		case DatumTypes::String:
			Set(value, index);
			break;
		case DatumTypes::Pointer:
			throw std::runtime_error("you can't set a pointer by string!");
			break;
		case DatumTypes::Unknown:
			throw std::runtime_error("you can't set an unknown type by string!");
			break;
		}
	}

	void Datum::PushBackFromString(std::string value)
	{
		switch (_type)
		{
		case DatumTypes::Integer:
			PushBack(std::stoi(value));
			break;
		case DatumTypes::Float:
			PushBack(std::stof(value));
			break;
		case DatumTypes::Vectors:
			float v1, v2, v3, v4;
			sscanf_s(value.c_str(), "vec4(%f,%f,%f,%f)", &v1, &v2, &v3, &v4);
			PushBack(glm::vec4{ v1, v2, v3, v4 });
			break;
		case DatumTypes::Matrix:
			float v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;
			sscanf_s(value.c_str(), "mat4x4((%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f),(%f,%f,%f,%f))", &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &v10, &v11, &v12, &v13, &v14, &v15, &v16);
			PushBack(glm::mat4(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16));
			break;
		case DatumTypes::String:
			PushBack(value);
			break;
		case DatumTypes::Pointer:
			throw std::runtime_error("you can't set a pointer by string!");
			break;
		case DatumTypes::Unknown:
			throw std::runtime_error("you can't set an unknown type by string!");
			break;
		}
	}

	void Datum::RemoveAt(const std::size_t index)
	{
		if (!_isInternal) throw std::runtime_error("You can't PopBack cause it's external!");

		switch (_type)
		{
		case DatumTypes::Integer:
			Remove(*(_data.i + index));
			break;
		case DatumTypes::Float:
			Remove(*(_data.f + index));
			break;
		case DatumTypes::Vectors:
			Remove(*(_data.v + index));
			break;
		case DatumTypes::Matrix:
			Remove(*(_data.m + index));
			break;
		case DatumTypes::String:
			Remove(*(_data.s + index));
			break;
		case DatumTypes::Pointer:
			Remove(*(_data.r + index));
			break;
		case DatumTypes::Table:
			Remove(*(_data.sc + index));
			break;
		default:
			throw std::runtime_error("you can't remove something from an unknown type");
		}
	}

	void Datum::PopBack()
	{
		if (!_isInternal) throw std::runtime_error("You can't PopBack cause it's external!");

		if (_size <= 0) return;

		_size--;
		if (_type == DatumTypes::String)
		{
			_data.s[_size].~basic_string();
		}
	}


	void Datum::SetStorage(std::int32_t* const other, std::size_t size)
	{
		if (_type!=DatumTypes::Unknown && _type !=DatumTypes::Integer) 
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}
		
		Clear();
		_type = DatumTypes::Integer;
		_isInternal = false;
		_data.i = other;

		_capacity = _size = size;
	}

	void Datum::SetStorage(float* const other, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Float)
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}

		Clear();
		_type = DatumTypes::Float;
		_isInternal = false;
		_data.f = other;

		_capacity = _size = size;
	}

	void Datum::SetStorage(glm::vec4* const other, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Vectors)
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}

		Clear();
		_type = DatumTypes::Vectors;
		_isInternal = false;
		_data.v = other;

		_capacity = _size = size;
	}

	void Datum::SetStorage(glm::mat4* const other, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Matrix)
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}

		Clear();
		_type = DatumTypes::Matrix;
		_isInternal = false;
		_data.m = other;

		_capacity = _size = size;
	}

	void Datum::SetStorage(std::string* const other, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::String)
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}

		Clear();
		_type = DatumTypes::String;
		_isInternal = false;
		_data.s = other;

		_capacity = _size = size;

	}

	void Datum::SetStorage(RTTI** const other, std::size_t size)
	{
		if (_type != DatumTypes::Unknown && _type != DatumTypes::Pointer)
		{
			throw std::runtime_error("you can't Set Storage to an existing object");
		}

		Clear();
		_type = DatumTypes::Pointer;
		_isInternal = false;
		_data.r = other;

		_capacity = _size = size;

	}

	void Datum::SetStorage(void* const other, std::size_t size, DatumTypes type)
	{
		if (type == DatumTypes::Unknown)
		{
			throw std::runtime_error("You can't pass in an unknown type!");
		}
		Clear();
		if (_isInternal && _capacity > 0)free(_data.vp);
		_type = type;
		_isInternal = false;
		_data.vp = other;

		_capacity = _size = size;

	}

	bool const Datum::IsInternal() const
	{
		return _isInternal;
	}

	Scope& Datum::operator[](std::size_t index)
	{
		return *Get<Scope*>(index);
	}
}