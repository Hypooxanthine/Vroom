#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"

#include "Vroom/Asset/AssetData/ShaderData.h"

namespace vrm
{

  class ShaderAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<ShaderAsset>;
  public:
    ShaderAsset() = default;
    ~ShaderAsset() = default;

    bool loadImpl(const std::string& filePath) override;

    const auto& getShaderData() const { return m_data; }

  private:
    ShaderData m_data;
  };

} // namespace vrm