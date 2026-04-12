#include "AssetManager/PathHelpers.h"

namespace fs = std::filesystem;

namespace vrm::PathHelpers
{

std::filesystem::path ConvertToAbsolute(const std::filesystem::path& path)
{
  if (path.is_absolute())
  {
    return path;
  }
  else
  {
    return fs::absolute(path);
  }
}

std::filesystem::path GetRelativeTo(const std::filesystem::path& parent, const std::filesystem::path& child)
{
  return child.lexically_normal().lexically_relative(parent.lexically_normal());
}

bool IsChildOf(const std::filesystem::path& parent, const std::filesystem::path& child)
{
  if (child == parent)
    return true;

  std::filesystem::path relative = GetRelativeTo(parent, child);
  return (!relative.empty() && relative.native()[0] != '.');
}

} // namespace vrm::PathHelpers
