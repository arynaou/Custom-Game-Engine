#pragma once
#include "pch.h"

namespace FieaGameEngine
{
	class DefaultIncrement
	{
	public:
		std::size_t operator()(std::size_t /*size*/, std::size_t capacity) const;
	};
}
#include "DefaultIncrement.inl"