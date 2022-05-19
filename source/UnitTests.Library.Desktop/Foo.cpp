#include "pch.h"
#include "Foo.h"

using namespace std;
using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo(int32_t data) :
		_data(new int32_t(data))
	{
	}

	Foo::Foo(const Foo& other) :
		_data(new int32_t(*other._data))
	{
	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*_data = *other._data;
		}
		return *this;
	}

	Foo::~Foo()
	{
		delete _data;
	}

	int32_t& Foo::Data()
	{
		return *_data;
	}

	int32_t& Foo::Data() const
	{
		return *_data;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (_data == rhs._data)
		{
			return true;
		}
		return (*_data == *rhs._data);
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}
}