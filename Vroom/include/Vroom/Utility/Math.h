#pragma once

#include <utility>

namespace Vroom::Math
{
	template <typename T>
	T Clamped(const T& value, const T& min, const T& max)
	{
		if (value > max) return max;
		else if (value < min) return min;
		else return value;
	}

	template <typename T>
	void Clamp(T& value, const T& min, const T& max)
	{
		T = std::move(Clamped(value, min, max));
	}
}