#pragma once

#include <glm/glm.hpp>
#include "Vroom/Render/ParticleEmitterField.h"

namespace vrm
{

  struct ParticleEmitterAttributeBase
  {
    // For editor usage
    enum class  EAttributeName
    {
      eColor = 0, ePosition, eScale
    };
    
    virtual ParticleEmitterFieldBase& getSpawnFieldBase() = 0;
    virtual ParticleEmitterFieldBase& getDeathFieldBase() = 0;

    inline const ParticleEmitterFieldBase& getSpawnFieldBase() const
    {
      return const_cast<ParticleEmitterAttributeBase*>(this)->getSpawnFieldBase();
    }

    inline const ParticleEmitterFieldBase& getDeathFieldBase() const
    {
      return const_cast<ParticleEmitterAttributeBase*>(this)->getDeathFieldBase();
    }
  };

  template <glm::length_t Dim>
  struct ParticleEmitterAttribute : public ParticleEmitterAttributeBase
  {
    using FieldType = ConstParticleEmitterField<Dim>;
    using VectorType = FieldType::VectorType;

    inline ParticleEmitterAttribute(const VectorType& singleValue) : spawnValue(singleValue), deathValue(singleValue) {}
    inline ParticleEmitterAttribute(const VectorType& spawn, const VectorType& death) : spawnValue(spawn), deathValue(death) {}
    inline ParticleEmitterAttribute() : ParticleEmitterAttribute(VectorType(0.f)) {}

    inline ParticleEmitterFieldBase& getSpawnFieldBase() override
    {
      return spawnValue;
    }

    inline ParticleEmitterFieldBase& getDeathFieldBase() override
    {
      return deathValue;
    }

    FieldType spawnValue;
    FieldType deathValue;
  };

}
