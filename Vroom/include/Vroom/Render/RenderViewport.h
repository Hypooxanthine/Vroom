#pragma once

#include <glm/glm.hpp>

namespace vrm
{
  
  class RenderViewport
  {
  public:

    inline RenderViewport() = default;

    inline RenderViewport(const glm::uvec2& origin, const glm::uvec2& size)
      : m_origin(origin), m_size(size)
    {}

    RenderViewport& operator=(const RenderViewport& other) = default;
    RenderViewport(const RenderViewport& other) = default;

    RenderViewport& operator=(RenderViewport&& other) = default;
    RenderViewport(RenderViewport&& other) = default;

    inline const glm::uvec2& getOrigin() const { return m_origin; }
    inline const glm::uvec2& getSize() const { return m_size; }

    inline void setOrigin(const glm::uvec2& origin) { m_origin = origin; }
    inline void setSize(const glm::uvec2& size) { m_size = size; }
    inline void setWidth(float width) { m_size.x = width; }
    inline void setHeight(float height) { m_size.y = height; }

  private:
    glm::uvec2 m_origin;
    glm::uvec2 m_size;
  };

} // namespace vrm
