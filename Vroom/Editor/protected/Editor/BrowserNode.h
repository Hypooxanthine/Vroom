#pragma once

#include <filesystem>
#include <vector>

namespace vrm
{

class BrowserNode
{
public:

  BrowserNode();
  BrowserNode(const std::filesystem::path& path, const std::filesystem::path& root);
  ~BrowserNode();

  bool                         ok() const;
  bool                         isDirectory() const;
  bool                         isFile() const;
  const std::filesystem::path& getPath() const;
  std::filesystem::path        getPathFromToRoot() const;
  const std::filesystem::path& getRoot() const;
  BrowserNode                  getParent() const;
  std::vector<BrowserNode>     getChildren() const;

  bool comparePath(const std::filesystem::path& other) const;

  void setPath(const std::filesystem::path& path, const std::filesystem::path& root);

private:

  std::filesystem::path _formatPath(const std::filesystem::path& path) const;
  void                  _invalidate();

private:

  std::filesystem::path m_root = "";
  std::filesystem::path m_path = "";
};

} // namespace vrm
