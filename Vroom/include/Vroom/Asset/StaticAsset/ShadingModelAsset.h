#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetData/ShaderData.h"

namespace vrm
{

  class ShadingModelAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<ShadingModelAsset>;

  public:
    ShadingModelAsset();
    ~ShadingModelAsset();

    [[nodiscard]] inline const ShaderData &getData() const { return m_data; }

  protected:
    bool loadImpl(const std::string &filePath) override;

  private:
    ShaderData m_data;
  };

} // namespace vrm
