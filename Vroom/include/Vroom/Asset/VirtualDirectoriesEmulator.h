#pragma once

#include <filesystem>
#include <vector>

#include "Vroom/Api.h"

namespace vrm
{

/**
 * @brief Helps retrieving a file on the disk with virtual
 * directories taken in account. For example, you can :
 * - add a "{any}/Resources/" virtual directory as "MyResources/"
 * - find "MyResources/meshes/myMesh.obj"
 *
 * It will retrieve the real path of myMesh.obj in the
 * "{any}/Resources/" hierarchy.
 *
 */
class VRM_API VirtualDirectoriesEmulator
{
public:

  VirtualDirectoriesEmulator();

  /**
   * @brief Add a virtual directory
   *
   * @param realDirPath Must be a path to a real directory. Otherwise does
   * nothing.
   * @param virtualPath Must be relative.
   *
   * @return True on success ; false otherwise.
   */
  bool addDirectory(const std::filesystem::path& realDirPath,
                    const std::filesystem::path& virtualPath);

  /**
   * @brief Add a virtual directory. If realDirPath is "{any}/foo/foo2/",
   * the virtual path is assumed to be "foo2/".
   *
   * @param realDirPath Must be a path to a real directory. Otherwise does
   * nothing.
   *
   * @return True on success ; false otherwise.
   */
  bool addDirectory(const std::filesystem::path& realDirPath);

  std::filesystem::path findPath(const std::filesystem::path& path) const;

  /**
   * @brief Get the children of a path.
   *
   * @param path Path to a directory. Either relative to working dir, or to a
   * virtual directory.
   * @return std::vector<std::filesystem::path> The children of the resolved
   * path. Empty on error.
   */
  std::vector<std::filesystem::path>
  getChildren(const std::filesystem::path& path) const;

private:

  struct VirtualDir
  {
    std::filesystem::path realPath;
    std::filesystem::path virtualPath;
  };

private:

  std::vector<VirtualDir> m_virtualDirs;
};

} // namespace vrm
