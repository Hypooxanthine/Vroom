#pragma once

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"

namespace vrm
{

  class MaterialDefines;
  
  class PassMaterial
  {
  public:
    PassMaterial();
    ~PassMaterial();

    PassMaterial& operator=(const PassMaterial& other) = delete;
    PassMaterial(const PassMaterial& other) = delete;

    PassMaterial& operator=(PassMaterial&& other) = delete;
    PassMaterial(PassMaterial&& other) = delete;

    void setMaterial(MaterialAsset::Handle material);
    
    void prepare(const MaterialDefines& defines);

    inline bool needsPrepare() const { return m_needsPrepare; }

  private:
    MaterialAsset::Handle m_materialAsset;
    bool m_needsPrepare = false;
  };

} // namespace vrm
