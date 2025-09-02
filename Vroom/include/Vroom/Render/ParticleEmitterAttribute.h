#pragma once

#include <variant>

#include <glm/glm.hpp>

#include "Vroom/Render/ParticleEmitterField.h"

namespace vrm
{

using ParticleEmitterFieldType =
  std::variant<ConstParticleEmitterField, RandomRangeEmitterField>;

struct ParticleEmitterAttribute
{
  using FieldType = ParticleEmitterFieldType;

  inline ParticleEmitterAttribute(const FieldType& singleValue)
    : spawnValue(singleValue), deathValue(singleValue)
  {}

  inline ParticleEmitterAttribute(const FieldType& spawn,
                                  const FieldType& death)
    : spawnValue(spawn), deathValue(death)
  {}

  inline ParticleEmitterAttribute()
    : ParticleEmitterAttribute(ConstParticleEmitterField())
  {}

  FieldType spawnValue;
  FieldType deathValue;
};

} // namespace vrm
