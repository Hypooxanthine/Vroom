#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include <algorithm>

using namespace vrm;

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
  if (loadImpl(filePath))
  {
    m_filePath = filePath;
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
