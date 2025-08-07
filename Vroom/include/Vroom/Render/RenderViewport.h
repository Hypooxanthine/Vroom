#pragma once

#include <glm/glm.hpp>

namespace vrm::render
{

  template <typename T>
  class ViewportBase
  {
  public:

    using SizeType = glm::vec<2, T>;

  public:

    inline ViewportBase() = default;

    inline ViewportBase(const SizeType& origin, const SizeType& size)
      : m_origin(origin), m_size(size)
    {}

    ViewportBase& operator=(const ViewportBase& other) = default;
    ViewportBase(const ViewportBase& other) = default;

    ViewportBase& operator=(ViewportBase&& other) = default;
    ViewportBase(ViewportBase&& other) = default;

    inline const SizeType& getOrigin() const { return m_origin; }
    inline T getOriginX() const { return m_origin.x; }
    inline T getOriginY() const { return m_origin.y; }
    inline const SizeType& getSize() const { return m_size; }
    inline T getWidth() const { return m_size.x; }
    inline T getHeight() const { return m_size.y; }

    inline void setOrigin(const SizeType& origin) { m_origin = origin; }
    inline void setOriginX(T x) { m_origin.x = x; }
    inline void setOriginY(T y) { m_origin.y = y; }
    inline void setSize(const SizeType& size) { m_size = size; }
    inline void setWidth(T width) { m_size.x = width; }
    inline void setHeight(T height) { m_size.y = height; }

  private:
    SizeType m_origin;
    SizeType m_size;
  };

  using Viewport = ViewportBase<glm::uint>;

} // namespace vrm::render
