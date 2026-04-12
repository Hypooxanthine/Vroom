#pragma once

#include "Rasterizer/Texture.h"
#include "RenderObjects/Api.h"

namespace vrm::render
{

class VRM_RENDEROBJECTS_API Cubemap
{
public:

  Cubemap()  = default;
  ~Cubemap() = default;

  inline void setTexture(gl::Texture&& texture)
  {
    m_texture = std::move(texture);
  }

  inline const gl::Texture& getTexture() const
  {
    return m_texture;
  }

private:

  gl::Texture m_texture;
};

} // namespace vrm::render
