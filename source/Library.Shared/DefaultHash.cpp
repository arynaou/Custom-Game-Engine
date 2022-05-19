#include "pch.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	std::size_t AdditiveHash(const std::uint8_t* data, size_t size)
	{
		std::size_t hash = 31;

		for (std::size_t i = 0; i < size; ++i)
		{
			hash += data[i];
		}

		return hash;
	}
}