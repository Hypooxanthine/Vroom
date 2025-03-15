#include "Vroom/Asset/StaticAsset/SceneAsset.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include "Vroom/Core/Assert.h"

namespace vrm
{

SceneAsset::SceneAsset()
{
}

SceneAsset::~SceneAsset()
{
}

SceneInstance SceneAsset::createInstance()
{
  return SceneInstance(this);
}

bool SceneAsset::loadImpl(const std::string &filePath)
{
  using json = nlohmann::json;

  std::ifstream ifs;
  ifs.open(filePath);
  if (!ifs.is_open())
    return false;

  m_SceneData.clear();
  ifs >> m_SceneData;

  return true;
}

} // namespace vrm