#pragma once
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace FieaGameEngine
{
	inline std::size_t AdditiveHash(const std::uint8_t* data, std::size_t size)
	{
		std::size_t hash = 29;

		for (std::size_t i = 0; i < size; ++i)
		{
			hash += data[i];
		}
		return hash;
	}

	template <typename T>
	struct DefaultHash
	{
		std::size_t operator()(const T& value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(&value), sizeof(T));
		}
	};

	template <>
	struct DefaultHash<std::string>
	{
		std::size_t operator()(const std::string& value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(value.c_str()), value.size());
		}
	};

	template <>
	struct DefaultHash<char*>
	{
		std::size_t operator()(const char* value)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(value), strlen(value));
		}
	};

}