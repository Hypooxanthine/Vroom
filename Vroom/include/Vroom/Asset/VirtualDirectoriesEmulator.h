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

  /**
   * @brief Representation of a virtual directory.

    If the virtual path is "my/virtual/dir/" and the real path is
   "real/path"/, accessing the virtual path "my/virtual/dir/resources/mesh.obj"
    will be resolved as "real/path/resources/mesh.obj"
   *
   */
  struct VirtualDir
  {
    /// @brief The real path of the virtual dir on disk.
    std::filesystem::path realPath;

    /**
     * @brief The virtual path: from which path it can be accessed.
     *
     */
    std::filesystem::path virtualPath;
  };

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

  /**
   * @brief Get the list of virtual directories added to the emulator.
   *
   * @return std::vector<VirtualDir> The virtual directories.
   */
  std::vector<VirtualDir> getDirectories() const;

  /**
   * @brief Resolve a real path from a virtual path.
   *
   * @param path The virtual path.
   * @return std::filesystem::path The resolved path. Empty if it could not be
   * resolved.
   */
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

  std::vector<VirtualDir> m_virtualDirs;
};

} // namespace vrm
