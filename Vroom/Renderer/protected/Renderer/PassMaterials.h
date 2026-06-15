#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "glm/gtx/hash.hpp"

#include "AssetManager/MaterialAsset.h"
#include "Rasterizer/Shader.h"

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

    PassMaterial& operator=(PassMaterial&& other) = default;
    PassMaterial(PassMaterial&& other) = default;

    void setMaterialAsset(MaterialAsset::Handle material);

    bool prepare(const MaterialDefines& defines);

    inline bool needsPrepare() const { return m_needsPrepare; }

    inline const gl::Shader& getShader() const
    {
      VRM_ASSERT(m_needsPrepare == false);
      return m_gpuShader;
    }

    size_t getGeneration() const;

  private:
    MaterialAsset::Handle m_materialAsset;
    gl::Shader m_gpuShader;
    bool m_needsPrepare = true;
    size_t m_generation = 1;
  };

  class PassMaterials
  {
  public:

    // A pass material is associated with a material asset, and a set of #define's.
    struct Key
    {
      MaterialAsset::Handle asset;
      std::map<std::string, std::string> defines;

      inline bool operator==(const Key& other) const
      {
        return asset == other.asset && defines == other.defines;
      }

      struct Hasher
      {
        inline size_t operator()(const Key& key) const
        {
          size_t seed = std::hash<uintptr_t>()(key.asset.getPtr());
          for (const auto& [k, v] : key.defines)
          {
            glm::detail::hash_combine(seed, std::hash<std::string>()(k));
            glm::detail::hash_combine(seed, std::hash<std::string>()(v));
          }
          return seed;
        }
      };
    };

    PassMaterials();
    ~PassMaterials();

    PassMaterials& operator=(const PassMaterials& other) = delete;
    PassMaterials(const PassMaterials& other) = delete;

    PassMaterials& operator=(PassMaterials&& other) = default;
    PassMaterials(PassMaterials&& other) = default;

    const PassMaterial& getMaterial(const Key& key);

    void releaseMaterial(const Key& key);

    void clear();

  private:

    std::unordered_map<Key, PassMaterial, Key::Hasher> m_materials;

  };

} // namespace vrm
