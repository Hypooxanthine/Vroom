#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include <filesystem>
#include <algorithm>

using namespace vrm;

StaticAsset::~StaticAsset()
{
  
}

void StaticAsset::notifyNewInstance() const
{
  m_InstanceCount++;
}

void StaticAsset::notifyDeleteInstance() const
{
  m_InstanceCount--;
}

size_t StaticAsset::getInstanceCount() const
{
  return m_InstanceCount;
}

bool StaticAsset::load(const std::string& filePath)
{
  m_filePath = filePath;
  
  if (loadImpl(applyPathOrder(filePath)))
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::string StaticAsset::getExtension(const std::string& filePath)
{
  // Finding the last dot
  size_t dotIndex = filePath.find_last_of('.');
  if (dotIndex == std::string::npos)
    return "";

  // Getting the extension
  std::string extension = filePath.substr(dotIndex + 1);

  // To lower case
  std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

  return extension;
}

std::string StaticAsset::applyPathOrder(const std::string& path) const
{
  std::filesystem::path assetDirAbs = std::filesystem::absolute(getFilePath());
  assetDirAbs.remove_filename();
  std::filesystem::path assetDirRel = assetDirAbs.lexically_relative(std::filesystem::current_path());
  std::filesystem::path relativePath = assetDirRel / path;

  if (std::filesystem::exists(relativePath)) // Relative to current asset first
  {
    return relativePath.string();
  }
  else // Relative to working directory then
  {
    return path;
  }
}
