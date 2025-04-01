#pragma once

#include <memory>

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetData/SceneData.h"

namespace vrm
{

  class SceneAsset : public StaticAsset
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