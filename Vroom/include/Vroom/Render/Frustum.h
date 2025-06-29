#pragma once

#include <array>
#include <span>
#include <glm/glm.hpp>

namespace vrm
{

  class Aabb;

  class Frustum
  {
  public:

    enum EXAxis      : uint8_t { eLeft = 0, eRight };
    enum EYAxis      : uint8_t { eDown = 0, eUp };
    enum EZAxis      : uint8_t { eNear = 0, eFar };

  public:

    /**
     * @brief Create a From Aabb object
     * 
     * @param aabb 
     * @param fromNDC If true: camera pointing +Z in left-handed coordinates system. If false: camera pointing -Z in right-handed coordinates system.
     */
    inline static constexpr Frustum CreateFromAabb(const Aabb& aabb, bool fromNDC);

  public:

    inline constexpr Frustum()
      : m_corners()
    {
    }

    inline ~Frustum() = default;

    inline constexpr std::span<const glm::vec3, 8> getCorners() const
    {
      return m_corners;
    }

    inline constexpr glm::vec3& corner(EXAxis x, EYAxis y, EZAxis z)
    {
      return m_corners.at(x + y * 2 + z * 4);
    }

    inline constexpr const glm::vec3& getCorner(EXAxis x, EYAxis y, EZAxis z) const
    {
      return m_corners.at(x + y * 2 + z * 4);
    }

    inline constexpr void transform(const glm::mat4& t)
    {
      for (glm::vec3& c : m_corners)
      {
        glm::vec4 c4 = t * glm::vec4(c, 1.f);
        c = c4 / c4.w;
      }
    }

  private:

    std::array<glm::vec3, 8> m_corners;

  };

} // namespace vrm

#include "Vroom/Render/Aabb.h"

inline constexpr vrm::Frustum vrm::Frustum::CreateFromAabb(const vrm::Aabb& aabb, bool fromNDC)
{
  std::array<glm::vec3, 2> minMax = { aabb.getMin(), aabb.getMax() };
  if (!fromNDC) // => if right-handed
    std::swap(minMax[0].z, minMax[1].z);

  vrm::Frustum f;
  
  for (uint8_t z = 0; z < 2; ++z)
  for (uint8_t y = 0; y < 2; ++y)
  for (uint8_t x = 0; x < 2; ++x)
    f.corner(static_cast<EXAxis>(x), static_cast<EYAxis>(y), static_cast<EZAxis>(z)) = glm::vec3{ minMax[x].x, minMax[y].y, minMax[z].z };

  return f;
}
