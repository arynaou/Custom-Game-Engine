#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	inline std::size_t DefaultIncrement::operator()(std::size_t /*size*/, std::size_t capacity) const
	{
		return capacity;
	}
}