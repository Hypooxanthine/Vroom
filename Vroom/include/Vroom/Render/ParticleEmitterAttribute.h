#pragma once

#include <glm/glm.hpp>
#include <span>
#include <type_traits>

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
    using VectorType = std::conditional_t<
      Dim == 1,
      float,
      glm::vec<Dim, float>
    >;

    inline ParticleEmitterField() = default;
    inline ParticleEmitterField(const VectorType& value) : value(value) {}

    inline std::span<float> getRawData() override
    {
      if constexpr (Dim > 1)
        return std::span{ &value[0], Dim };
      else
        return std::span{ &value, Dim };
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
