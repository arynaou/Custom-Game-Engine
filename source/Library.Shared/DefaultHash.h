#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	std::size_t AdditiveHash(const std::uint8_t* data, size_t size);

	template <typename TKey>
	struct DefaultHash
	{
		std::size_t operator()(const TKey& key);
	};

	template <>
	struct DefaultHash<std::string>
	{
		std::size_t operator()(const std::string& key);
	};

	template <>
	struct DefaultHash<const std::string>
	{
		std::size_t operator()(const std::string& key);
	};

	template <>
	struct DefaultHash<char *>
	{
		std::size_t operator()(const char* key);
	};

	template <>
	struct DefaultHash<const char*>
	{
		std::size_t operator()(const char* key);
	};
}

#include "DefaultHash.inl"