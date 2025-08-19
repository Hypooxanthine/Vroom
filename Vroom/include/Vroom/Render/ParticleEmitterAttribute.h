#pragma once

#include <glm/glm.hpp>
#include <span>

namespace vrm
{

  struct ParticleEmitterFieldBase
  {
    virtual std::span<float> getRawData() = 0;
    inline std::span<float const> getRawData() const
    {
      return const_cast<ParticleEmitterFieldBase*>(this)->getRawData();
    }
  };

  template <glm::length_t Dim>
  struct ParticleEmitterField : public ParticleEmitterFieldBase
  {
    using VectorType = glm::vec<Dim, float>;

    inline ParticleEmitterField() = default;
    inline ParticleEmitterField(const VectorType& value) : value(value) {}

    std::span<float> getRawData() override
    {
      return std::span{ &value[0], Dim };
    }

    inline operator VectorType() const { return value; }

    VectorType value;

  };

  struct ParticleEmitterAttributeBase
  {
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
    using FieldType = ParticleEmitterField<Dim>;
    using VectorType = FieldType::VectorType;

    ParticleEmitterAttribute(const VectorType& singleValue) : spawnValue(singleValue), deathValue(singleValue) {}
    ParticleEmitterAttribute(const VectorType& spawn, const VectorType& death) : spawnValue(spawn), deathValue(death) {}
    ParticleEmitterAttribute() : ParticleEmitterAttribute(VectorType(0.f)) {}

    ParticleEmitterFieldBase& getSpawnFieldBase() override
    {
      return spawnValue;
    }

    ParticleEmitterFieldBase& getDeathFieldBase() override
    {
      return deathValue;
    }

    FieldType spawnValue;
    FieldType deathValue;
  };

}
