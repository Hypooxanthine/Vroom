#include "VroomEditor/UserInterface/AssetBrowser/BrowserNode.h"
#include <filesystem>

using namespace vrm;

BrowserNode::BrowserNode()
{}

BrowserNode::BrowserNode(const std::filesystem::path& path, const std::filesystem::path& root)
{
  setPath(path, root);
}

BrowserNode::~BrowserNode()
{}

bool BrowserNode::ok() const
{
  return std::filesystem::exists(m_path) && std::filesystem::exists(m_root);
}

bool BrowserNode::isDirectory() const
{
  return std::filesystem::exists(m_path) && std::filesystem::is_directory(m_path);
}

bool BrowserNode::isFile() const
{
  return std::filesystem::exists(m_path) && std::filesystem::is_regular_file(m_path);
}

const std::filesystem::path& BrowserNode::getPath() const
{
  return m_path;
}

std::filesystem::path BrowserNode::getPathFromToRoot() const
{
  return m_path.lexically_relative(m_root.parent_path());
}

const std::filesystem::path& BrowserNode::getRoot() const
{
  return m_root;
}

BrowserNode BrowserNode::getParent() const
{
  if (!ok() || m_path == m_root)
    return {};

  return BrowserNode(m_path.parent_path(), m_root);
}

std::vector<BrowserNode> BrowserNode::getChildren() const
{
  std::vector<BrowserNode> nodes;

  // Only for directory nodes
  if (!ok() || !isDirectory())
    return nodes;

  for (auto it : std::filesystem::directory_iterator(m_path))
  {
    nodes.emplace_back(it.path(), m_root);
  }

  return nodes;
}

bool BrowserNode::comparePath(const std::filesystem::path& other) const
{
  return m_path == _formatPath(other);
}

void BrowserNode::setPath(const std::filesystem::path& path, const std::filesystem::path& root)
{
  if (!std::filesystem::exists(path) || !std::filesystem::exists(root))
  {
    _invalidate();
    return;
  }

  auto newPath = _formatPath(path);
  auto newRoot = _formatPath(root);

  m_path = newPath;
  m_root = newRoot;
}

std::filesystem::path BrowserNode::_formatPath(const std::filesystem::path& path) const
{
  std::filesystem::path formatted;

  if (path.is_relative())
    formatted = std::filesystem::absolute(path);
  else
    formatted = path;

  formatted = std::filesystem::canonical(formatted);

  return formatted;
}

void BrowserNode::_invalidate()
{
  m_path = "";
  m_root = "";
}
