#pragma once

#include <algorithm>
#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"
#include "glm/fwd.hpp"

namespace vrm
{

enum class EmitterFieldType
{
  eConst = 0,
  eRandomRange,
  eRandomCone
};

class IEmitterField
{
public:

  inline bool structureDifferent(const IEmitterField& other) const
  {
    return getType() != other.getType();
  }

  virtual IEmitterField*   clone() const                                    = 0;
  virtual EmitterFieldType getType() const                                  = 0;
  virtual void             applyDefines(MaterialDefines&   defines,
                                        const std::string& prefix)          = 0;
  virtual void             pushToLayout(render::SSBO430Layout& layout)      = 0;
  virtual void assignStructuredBufferData(render::StructuredBuffer& buffer) = 0;
  virtual glm::vec4 getUpperBound() const { return glm::vec4(0.f); };
};

using EmitterFieldPtr = std::unique_ptr<IEmitterField>;

class IConstEmitterField : public IEmitterField
{
public:

  virtual void setValue(std::span<float const> value) = 0;
};

template <glm::length_t Dim>
class ConstEmitterField : public IConstEmitterField
{
public:

  using VectorType = glm::vec<Dim, float>;

public:

  ConstEmitterField()  = default;
  ~ConstEmitterField() = default;

  ConstEmitterField(const VectorType& value) : m_value(value) {}

  ConstEmitterField& operator=(const ConstEmitterField& other) = default;
  ConstEmitterField(const ConstEmitterField& other)            = default;

  ConstEmitterField& operator=(ConstEmitterField&& other) = default;
  ConstEmitterField(ConstEmitterField&& other)            = default;

  inline void setValue(const VectorType& value) { m_value = value; }

  inline const VectorType& getValue() const { return m_value; }

  ConstEmitterField* clone() const override
  {
    return new ConstEmitterField(*this);
  }

  EmitterFieldType getType() const override { return EmitterFieldType::eConst; }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    defines.add(prefix + "_Const");
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_attrib = layout.push<VectorType>();
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    buffer.setAttribute(m_value, m_attrib, 0);
  }

  glm::vec4 getUpperBound() const override
  {
    glm::vec4 out(0.f);
    std::copy_n(&m_value.x, Dim, &out.x);
    return out;
  }

  void setValue(std::span<float const> value) override
  {
    std::copy_n(value.begin(), std::min<size_t>(Dim, value.size()), &m_value.x);
  }

private:

  VectorType                    m_value;
  render::SSBO430Layout::Attrib m_attrib;
};

using ConstEmitterField1 = ConstEmitterField<1>;
using ConstEmitterField2 = ConstEmitterField<2>;
using ConstEmitterField3 = ConstEmitterField<3>;
using ConstEmitterField4 = ConstEmitterField<4>;

class IRandomRangeEmitterField : public IEmitterField
{
public:

  virtual void setValue(std::span<float const> minValue,
                        std::span<float const> maxValue) = 0;
};

template <glm::length_t Dim>
class RandomRangeEmitterField : public IRandomRangeEmitterField
{
public:

  using VectorType = glm::vec<Dim, float>;

public:

  RandomRangeEmitterField()  = default;
  ~RandomRangeEmitterField() = default;

  RandomRangeEmitterField(const VectorType& min, const VectorType& max)
    : m_minValue(min), m_maxValue(max)
  {}

  RandomRangeEmitterField&
  operator=(const RandomRangeEmitterField& other)               = default;
  RandomRangeEmitterField(const RandomRangeEmitterField& other) = default;

  RandomRangeEmitterField& operator=(RandomRangeEmitterField&& other) = default;
  RandomRangeEmitterField(RandomRangeEmitterField&& other)            = default;

  inline void setMinValue(const VectorType& value) { m_minValue = value; }

  inline void setMaxValue(const VectorType& value) { m_maxValue = value; }

  inline const VectorType& getMinValue() const { return m_minValue; }
  inline const VectorType& getMaxValue() const { return m_maxValue; }

  RandomRangeEmitterField* clone() const override
  {
    return new RandomRangeEmitterField(*this);
  }

  EmitterFieldType getType() const override
  {
    return EmitterFieldType::eRandomRange;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    defines.add(prefix + "_RandomRange");
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_minAttrib = layout.push<VectorType>();
    m_maxAttrib = layout.push<VectorType>();
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    buffer.setAttribute(m_minValue, m_minAttrib, 0);
    buffer.setAttribute(m_maxValue, m_maxAttrib, 0);
  }

  glm::vec4 getUpperBound() const override
  {
    glm::vec4 out(0.f);
    std::copy_n(&m_maxValue.x, Dim, &out.x);
    return out;
  }

  void setValue(std::span<float const> minValue,
                std::span<float const> maxValue) override
  {
    std::copy_n(minValue.begin(), std::min<size_t>(Dim, minValue.size()),
                &m_minValue.x);
    std::copy_n(maxValue.begin(), std::min<size_t>(Dim, maxValue.size()),
                &m_maxValue.x);
  }

private:

  VectorType m_minValue;
  VectorType m_maxValue;

  render::SSBO430Layout::Attrib m_minAttrib;
  render::SSBO430Layout::Attrib m_maxAttrib;
};

using RandomRangeEmitterField1 = RandomRangeEmitterField<1>;
using RandomRangeEmitterField2 = RandomRangeEmitterField<2>;
using RandomRangeEmitterField3 = RandomRangeEmitterField<3>;
using RandomRangeEmitterField4 = RandomRangeEmitterField<4>;

class RandomConeEmitterField : public IEmitterField
{
public:

  RandomConeEmitterField()  = default;
  ~RandomConeEmitterField() = default;

  RandomConeEmitterField(const glm::vec3& direction, float angle, float length)
    : m_direction(direction), m_angle(angle), m_length(length)
  {}

  RandomConeEmitterField&
  operator=(const RandomConeEmitterField& other)              = default;
  RandomConeEmitterField(const RandomConeEmitterField& other) = default;

  RandomConeEmitterField& operator=(RandomConeEmitterField&& other) = default;
  RandomConeEmitterField(RandomConeEmitterField&& other)            = default;

  inline void setDirection(const glm::vec3& direction)
  {
    m_direction = direction;
  }

  inline void setAngle(float angle) { m_angle = angle; }
  inline void setLength(float length) { m_length = length; }

  inline const glm::vec3& getDirection() const { return m_direction; }
  inline float            getAngle() const { return m_angle; }
  inline float            getLength() const { return m_length; }

  RandomConeEmitterField* clone() const override
  {
    return new RandomConeEmitterField(*this);
  }

  EmitterFieldType getType() const override
  {
    return EmitterFieldType::eRandomCone;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    defines.add(prefix + "_RandomCone");
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_directionAttrib = layout.push<glm::vec3>();
    m_angleAttrib     = layout.push<float>();
    m_lengthAttrib    = layout.push<float>();
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    buffer.setAttribute(m_direction, m_directionAttrib, 0);
    buffer.setAttribute(m_angle, m_angleAttrib, 0);
    buffer.setAttribute(m_length, m_lengthAttrib, 0);
  }

private:

  glm::vec3 m_direction;
  float     m_angle;
  float     m_length;

  render::SSBO430Layout::Attrib m_directionAttrib;
  render::SSBO430Layout::Attrib m_angleAttrib;
  render::SSBO430Layout::Attrib m_lengthAttrib;
};

} // namespace vrm
