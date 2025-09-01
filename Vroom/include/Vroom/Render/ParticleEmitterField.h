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
  struct ConstParticleEmitterField : public ParticleEmitterFieldBase
  {
    using VectorType = std::conditional_t<
      Dim == 1,
      float,
      glm::vec<Dim, float>
    >;

    inline ConstParticleEmitterField() = default;
    inline ConstParticleEmitterField(const VectorType& value) : value(value) {}

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
}