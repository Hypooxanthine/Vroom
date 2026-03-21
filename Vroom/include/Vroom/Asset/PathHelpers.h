#pragma once

#include <filesystem>

#include "Vroom/Api.h"

namespace vrm::PathHelpers
{

VRM_API std::filesystem::path ConvertToAbsolute(const std::filesystem::path& path);

VRM_API std::filesystem::path GetRelativeTo(const std::filesystem::path& parent, const std::filesystem::path& child);

VRM_API bool IsChildOf(const std::filesystem::path& parent, const std::filesystem::path& child);

}
