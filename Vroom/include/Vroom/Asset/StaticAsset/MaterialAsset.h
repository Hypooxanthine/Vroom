#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/StaticAsset/TextureAsset.h"
#include "Vroom/Render/Abstraction/Shader.h"

#include <fstream>
#include <vector>

namespace vrm
{

  class MaterialAsset : public StaticAsset
  {
  public:
    using Handle = AssetHandle<MaterialAsset>;
  public:
    MaterialAsset();
    ~MaterialAsset();

    /**
     * @brief Get the shader of the material.
     *
     * @return const Shader& The shader.
     */
    [[nodiscard]] inline const gl::Shader &getShader() const { return m_Shader; }

    /**
     * @brief Get the number of textures in the material.
     *
     * @return size_t The number of textures.
     */
    [[nodiscard]] inline size_t getTextureCount() const { return m_Textures.size(); }

    /**
     * @brief Get a texture from the material.
     *
     * @param slot The slot of the texture.
     * @return const TextureInstance& The texture.
     */
    [[nodiscard]] inline const TextureAsset::Handle &getTexture(size_t slot) const { return m_Textures[slot]; }

  protected:
    bool loadImpl(const std::string &filePath) override;

  private:
    gl::Shader m_Shader;
    std::vector<TextureAsset::Handle> m_Textures;
  };

} // namespace vrm
