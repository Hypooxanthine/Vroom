#pragma once

#include <filesystem>

#include "AssetManager/Api.h"

namespace vrm::PathHelpers
{

VRM_ASSETMANAGER_API std::filesystem::path ConvertToAbsolute(const std::filesystem::path& path);

VRM_ASSETMANAGER_API std::filesystem::path GetRelativeTo(const std::filesystem::path& parent, const std::filesystem::path& child);

VRM_ASSETMANAGER_API bool IsChildOf(const std::filesystem::path& parent, const std::filesystem::path& child);

}
