#include "Vroom/Asset/StaticAsset/SceneAsset.h"

#include <nlohmann/json.hpp>

#include "Vroom/Core/Log.h"

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

  return false;
}

} // namespace vrm