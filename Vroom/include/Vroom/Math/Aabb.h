#pragma once

#include <glm/glm.hpp>
#include <span>

namespace vrm
{

  class Frustum;

  class Aabb
  {
  public:

    inline static constexpr Aabb CreateFromFrustum(const Frustum& frustum);

  public:

    template <typename It>
      requires std::convertible_to<std::iter_value_t<It>, glm::vec3>
    inline constexpr Aabb(It begin, It end)
    {
      addPoints(begin, end);
    }

    inline constexpr Aabb()
    {
    }

    ~Aabb() = default;

    inline constexpr Aabb(const Aabb&) = default;
    inline constexpr Aabb& operator=(const Aabb&) = default;
    inline constexpr Aabb(Aabb&&) = default;
    inline constexpr Aabb& operator=(Aabb&&) = default;

    inline constexpr const glm::vec3& getMin() const { return m_min; }
    inline constexpr const glm::vec3& getMax() const { return m_max; }

    inline constexpr glm::vec3 calcMinToMaxVector() const { return getMax() - getMin(); }
    inline constexpr glm::vec3 calcCenter() const { return calcMinToMaxVector() / 2.f; }
    inline constexpr float calcWidth() const  { return getMax().x - getMin().x; }
    inline constexpr float calcHeight() const { return getMax().y - getMin().y; }
    inline constexpr float calcDepth() const  { return getMax().z - getMin().z; }

    inline constexpr void setMin(const glm::vec3& p) { m_min = p; }
    inline constexpr void setMax(const glm::vec3& p) { m_max = p; }

    inline constexpr void addPoint(const glm::vec3& p)
    {
      m_min = glm::min(m_min, p);
      m_max = glm::max(m_max, p);
    }

    template <typename It>
      requires std::convertible_to<std::iter_value_t<It>, glm::vec3>
    inline constexpr void addPoints(It begin, It end)
    {
      while (begin != end)
      {
        addPoint(*begin);
        ++begin;
      }
    }

  public:

    inline static constexpr Aabb GetNDC()
    {
      Aabb out;
      out.addPoint(glm::vec3{ -1.f, -1.f, -1.f });
      out.addPoint(glm::vec3{  1.f,  1.f,  1.f });
      return out;
    }

  private:
    glm::vec3 m_min = glm::vec3{ std::numeric_limits<float>::max() };
    glm::vec3 m_max = glm::vec3{ std::numeric_limits<float>::min() };
  };

}

#include "Vroom/Math/Frustum.h"

inline constexpr vrm::Aabb vrm::Aabb::CreateFromFrustum(const vrm::Frustum& frustum)
{
  return vrm::Aabb(frustum.getCorners().begin(), frustum.getCorners().end());
}
