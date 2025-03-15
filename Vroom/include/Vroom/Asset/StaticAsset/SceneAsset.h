#pragma once

#include <nlohmann/json.hpp>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/SceneInstance.h"

namespace vrm
{

class SceneAsset : public StaticAsset
{
public:
  using InstanceType = SceneInstance;

public:
  SceneAsset();
  ~SceneAsset();

  [[nodiscard]] SceneInstance createInstance();

  const nlohmann::json& getSceneData() const { return m_SceneData; }

protected:
  bool loadImpl(const std::string &filePath) override;

private:
  nlohmann::json m_SceneData;
  
};

} // namespace vrm