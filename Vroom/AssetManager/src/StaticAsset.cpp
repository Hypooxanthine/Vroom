#include "AssetManager/StaticAsset.h"

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
    ++m_generation;
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

std::filesystem::path StaticAsset::applyPathOrder(const std::filesystem::path& path) const
{
  std::filesystem::path relativePath = (getFilePath().parent_path() / path).lexically_normal();

  if (std::filesystem::exists(relativePath)) // Relative to current asset first
  {
    return relativePath.string();
  }
  else // Relative to working directory then
  {
    return path;
  }
}

size_t StaticAsset::getGeneration() const
{
  return m_generation;
}
