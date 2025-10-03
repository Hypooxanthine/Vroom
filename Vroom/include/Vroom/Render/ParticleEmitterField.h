#pragma once

#include <algorithm>
#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"

namespace vrm
{

enum class EmitterFieldType
{
  Const = 0,
  RandomRange
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
  virtual glm::vec4 getUpperBound() const                                   = 0;
};

using EmitterField1 = IEmitterField;
using EmitterField2 = IEmitterField;
using EmitterField3 = IEmitterField;
using EmitterField4 = IEmitterField;

template <glm::length_t Dim>
using EmitterFieldPtr = std::unique_ptr<IEmitterField>;

using EmitterField1Ptr = EmitterFieldPtr<1>;
using EmitterField2Ptr = EmitterFieldPtr<2>;
using EmitterField3Ptr = EmitterFieldPtr<3>;
using EmitterField4Ptr = EmitterFieldPtr<4>;

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

  EmitterFieldType getType() const override { return EmitterFieldType::Const; }

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
    return EmitterFieldType::RandomRange;
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

} // namespace vrm
