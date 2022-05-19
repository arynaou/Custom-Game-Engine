#include "pch.h"
#include "Signature.h"

namespace FieaGameEngine
{
	Signature::Signature(std::string name, DatumTypes type, std::size_t size, std::size_t offset):
		_name(name), _type(type), _size(size), _offset(offset)
	{
	}
}