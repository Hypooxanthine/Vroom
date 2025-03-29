#pragma once

#include "Vroom/Asset/StaticAsset/StaticAsset.h"
#include "Vroom/Asset/AssetInstance/MaterialInstance.h"
#include "Vroom/Asset/AssetInstance/TextureInstance.h"

#include "Vroom/Render/Abstraction/Shader.h"

#include <fstream>
#include <vector>

namespace vrm
{

  class MaterialAsset : public StaticAsset
  {
  public:
    using InstanceType = MaterialInstance;

  public:
    MaterialAsset();
    ~MaterialAsset();

    /**
     * @brief Create a new instance of the material.
     *
     * @return MaterialInstance The new instance.
     */
    [[nodiscard]] MaterialInstance createInstance();

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
    [[nodiscard]] inline const TextureInstance &getTexture(size_t slot) const { return m_Textures[slot]; }

  protected:
    bool loadImpl(const std::string &filePath) override;

  private:
    gl::Shader m_Shader;
    std::vector<TextureInstance> m_Textures;
  };

} // namespace vrm
