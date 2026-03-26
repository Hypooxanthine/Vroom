#include "Vroom/Asset/VirtualDirectoriesEmulator.h"
#include <filesystem>
#include <vector>

#include "Vroom/Asset/PathHelpers.h"
#include "Vroom/Core/Assert.h"

using namespace vrm;
namespace fs = std::filesystem;

VirtualDirectoriesEmulator::VirtualDirectoriesEmulator()
{}

bool VirtualDirectoriesEmulator::addDirectory(const fs::path& realDirPath,
                                              const fs::path& virtualPath)
{
  VRM_CHECK_RET_FALSE_MSG(fs::exists(realDirPath),
                          "Added root {} does not exist", realDirPath.string());
  VRM_CHECK_RET_FALSE_MSG(fs::is_directory(realDirPath),
                          "Added root {} is not a directory",
                          realDirPath.string());
  VRM_CHECK_RET_FALSE_MSG(virtualPath.is_relative(),
                          "Virtual must be relative");

  m_virtualDirs.emplace_back(
    VirtualDir{ PathHelpers::ConvertToAbsolute(realDirPath).lexically_normal(),
                virtualPath });

  return true;
}

bool VirtualDirectoriesEmulator::addDirectory(const fs::path& realDirPath)
{
  fs::path virtualPath = realDirPath.lexically_normal().filename();
  return addDirectory(realDirPath, virtualPath);
}

std::vector<VirtualDirectoriesEmulator::VirtualDir>
VirtualDirectoriesEmulator::getDirectories() const
{
  return m_virtualDirs;
}

fs::path VirtualDirectoriesEmulator::findPath(const fs::path& path) const
{
  fs::path result;

  if (fs::exists(path))
    result = path;
  else
  {
    for (const auto& virtualDir : m_virtualDirs)
    {
      fs::path virtualDirAbsolutePath =
        fs::current_path() / virtualDir.virtualPath;
      fs::path virtualAbsolutePath = PathHelpers::ConvertToAbsolute(path);

      if (PathHelpers::IsChildOf(virtualDirAbsolutePath, virtualAbsolutePath))
      {
        result = virtualDir.realPath
               / PathHelpers::GetRelativeTo(virtualDirAbsolutePath,
                                            virtualAbsolutePath);
      }
    }
  }
  return result.lexically_normal();
}

std::vector<fs::path>
VirtualDirectoriesEmulator::getChildren(const fs::path& path) const
{
  std::vector<fs::path> result;
  fs::path              resolvedPath = findPath(path);

  VRM_CHECK_RETVAL_MSG(fs::exists(resolvedPath), result,
                       "Resolved path {} does not exist",
                       resolvedPath.string());
  VRM_CHECK_RETVAL_MSG(fs::is_directory(resolvedPath), result,
                       "{} is not a directory", resolvedPath.string());

  for (const auto& child : fs::directory_iterator(resolvedPath))
  {
    result.emplace_back(child);
  }

  return result;
}
