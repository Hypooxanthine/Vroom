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

    void setMaterialAsset(MaterialAsset::Handle material);
    
    bool prepare(const MaterialDefines& defines);

    inline bool needsPrepare() const { return m_needsPrepare; }

    inline const gl::Shader& getShader() const
    {
      VRM_ASSERT(m_needsPrepare == false);
      return m_gpuShader;
    }

  private:
    MaterialAsset::Handle m_materialAsset;
    gl::Shader m_gpuShader;
    bool m_needsPrepare = true;
  };

} // namespace vrm
