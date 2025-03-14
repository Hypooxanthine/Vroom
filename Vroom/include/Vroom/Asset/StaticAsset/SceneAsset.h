#pragma once

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

protected:
  bool loadImpl(const std::string &filePath) override;

private:
  
};

} // namespace vrm