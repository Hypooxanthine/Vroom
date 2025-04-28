#pragma once

#include <glm/glm.hpp>

namespace vrm
{
  
  class RenderViewport
  {
  public:

    inline RenderViewport(const glm::uvec2& origin, const glm::uvec2& size)
      : m_origin(origin), m_size(size)
    {}

    inline const glm::uvec2& getOrigin() const { return m_origin; }
    inline const glm::uvec2& getSize() const { return m_size; }

    inline void setOrigin(const glm::uvec2& origin) { m_origin = origin; }
    inline void setSize(const glm::uvec2& size) { m_size = size; }

  private:
    glm::uvec2 m_origin;
    glm::uvec2 m_size;
  };

} // namespace vrm
