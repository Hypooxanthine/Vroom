#pragma once

#include "Vroom/Asset/StaticAsset/MaterialAsset.h"
#include "Vroom/Render/Abstraction/Shader.h"

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
    
    bool prepare(const MaterialDefines& defines);

    inline bool needsPrepare() const { return m_needsPrepare; }

  private:
    MaterialAsset::Handle m_materialAsset;
    gl::Shader m_gpuShader;
    bool m_needsPrepare = false;
  };

} // namespace vrm
