#pragma once

#include <string>
#include <variant>

#include <glm/glm.hpp>

#include "Vroom/Render/ParticleEmitterField.h"

namespace vrm
{

using ParticleEmitterFieldType =
  std::variant<ConstParticleEmitterField, RandomRangeEmitterField>;

class ParticleEmitterAttribute
{
public:

  using FieldType = ParticleEmitterFieldType;

  FieldType spawnValue;
  FieldType deathValue;

  inline ParticleEmitterAttribute(const std::string& shaderDefineName,
                                  const FieldType&   spawn,
                                  const FieldType&   death)
    : spawnValue(spawn), deathValue(death), m_shaderDefineName(shaderDefineName)
  {}

  inline ParticleEmitterAttribute(const std::string& shaderDefineName,
                                  const FieldType&   singleValue)
    : ParticleEmitterAttribute(shaderDefineName, singleValue, singleValue)
  {}

  inline ParticleEmitterAttribute(const std::string& shaderDefineName)
    : ParticleEmitterAttribute(shaderDefineName, ConstParticleEmitterField())
  {}

  ParticleEmitterAttribute&
  operator=(const ParticleEmitterAttribute& other)                = default;
  ParticleEmitterAttribute(const ParticleEmitterAttribute& other) = default;

  ParticleEmitterAttribute&
  operator=(ParticleEmitterAttribute&& other)                = default;
  ParticleEmitterAttribute(ParticleEmitterAttribute&& other) = default;

  inline bool structureDifferent(const ParticleEmitterAttribute& other) const
  {
    return spawnValue.index() != other.spawnValue.index()
        && spawnValue.index() != other.spawnValue.index();
  }

  const std::string& getShaderDefineName() const { return m_shaderDefineName; }

private:

  // No VRM_ prefix, no trailing/tailing underscores
  std::string m_shaderDefineName;
};

} // namespace vrm
