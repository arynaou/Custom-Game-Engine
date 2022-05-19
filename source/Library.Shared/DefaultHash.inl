#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename TKey>
	inline std::size_t DefaultHash<TKey>::operator()(const TKey& key)
	{
		return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&key), sizeof(TKey));
	}

	inline std::size_t DefaultHash<std::string>::operator()(const std::string& key)
	{
		return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key.c_str()), key.size());
	}

	inline std::size_t DefaultHash<const std::string>::operator()(const std::string& key)
	{
		return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key.c_str()), key.size());
	}

	inline std::size_t DefaultHash<char*>::operator()(const char* key)
	{
		return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key), strlen(key));
	}

	inline std::size_t DefaultHash<const char*>::operator()(const char* key)
	{
		return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key), strlen(key));
	}
}