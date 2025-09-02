#pragma once

#include <glm/glm.hpp>

namespace vrm
{

struct ConstParticleEmitterField
{
  inline ConstParticleEmitterField() = default;
  inline ConstParticleEmitterField(const glm::vec4& value) : value(value) {}

  glm::vec4 value;
};

struct RandomRangeEmitterField
{
  inline RandomRangeEmitterField() = default;
  inline RandomRangeEmitterField(const glm::vec4& minValue,
                                 const glm::vec4& maxValue)
    : minValue(minValue), maxValue(maxValue)
  {}

  glm::vec4 minValue, maxValue;
};

} // namespace vrm
