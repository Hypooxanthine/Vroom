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

  inline bool structureDifferent(const ParticleEmitterAttribute& other) const
  {
    return spawnValue.index() != other.spawnValue.index()
        && spawnValue.index() != other.spawnValue.index();
  }

  FieldType spawnValue;
  FieldType deathValue;
};

} // namespace vrm
