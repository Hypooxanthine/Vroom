#pragma once

#include "Vroom/Core/Application.h"
#include "Vroom/Core/Minimal.h"

namespace Vroom::Assets
{
	template <typename T>
	const T* GetAsset(const std::string& path)
	{
		static_assert(std::is_base_of<StaticAsset, T>::value);

		return Application::Get().getAssetManager().getStaticAsset<T>(path);
	}
}