#pragma once

#include <memory>

#include "AssetManager/Api.h"
#include "AssetManager/StaticAsset.h"
#include "AssetManager/SceneData.h"

namespace vrm
{

  class VRM_ASSETMANAGER_API SceneAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<SceneAsset>;
  public:
    SceneAsset();
    ~SceneAsset();

    const SceneData& getSceneData() const { return *m_data; }

  protected:
    bool loadImpl(const std::string& filePath) override;

  private:
    std::unique_ptr<SceneData> m_data;
  };

} // namespace vrm