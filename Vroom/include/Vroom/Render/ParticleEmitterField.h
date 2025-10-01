#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"

namespace vrm
{

struct ConstParticleEmitterField
{
  inline ConstParticleEmitterField() = default;
  inline ConstParticleEmitterField(const glm::vec4& value) : value(value) {}

  glm::vec4 value;
};

struct RandomRangeParticleEmitterField
{
  inline RandomRangeParticleEmitterField() = default;
  inline RandomRangeParticleEmitterField(const glm::vec4& minValue,
                                         const glm::vec4& maxValue)
    : minValue(minValue), maxValue(maxValue)
  {}

  glm::vec4 minValue, maxValue;
};

class EmitterField
{
public:

  EmitterField(size_t dim) : m_dim(dim) {}
  ~EmitterField() = default;

  EmitterField& operator=(const EmitterField& other) = delete;
  EmitterField(const EmitterField& other)            = delete;

  EmitterField& operator=(EmitterField&& other) = delete;
  EmitterField(EmitterField&& other)            = delete;

  inline size_t getDim() const { return m_dim; }

  virtual void applyDefines(MaterialDefines&   defines,
                            const std::string& prefix)                      = 0;
  virtual void pushToLayout(render::SSBO430Layout& layout)                  = 0;
  virtual void assignStructuredBufferData(render::StructuredBuffer& buffer) = 0;

private:

  size_t m_dim;
};

using EmitterFieldPtr = std::unique_ptr<EmitterField>;

class ConstEmitterField : public EmitterField
{
public:

  ConstEmitterField(size_t dim) : EmitterField(dim) {}
  ~ConstEmitterField() = default;

  ConstEmitterField& operator=(const ConstEmitterField& other) = delete;
  ConstEmitterField(const ConstEmitterField& other)            = delete;

  ConstEmitterField& operator=(ConstEmitterField&& other) = delete;
  ConstEmitterField(ConstEmitterField&& other)            = delete;

  template <glm::length_t Dim>
  inline void setValue(const glm::vec<Dim, float>& value)
  {
    m_value = glm::vec4(value);
  }

  inline const glm::vec4& getValue() const { return m_value; }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    defines.add(prefix + "_Const");
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_attrib = layout.pushVectorFloat(getDim());
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    buffer.setAttribute(std::span{ &m_value.x, getDim() }, m_attrib, 0);
  }

private:

  glm::vec4                     m_value;
  render::SSBO430Layout::Attrib m_attrib;
};

class RandomRangeEmitterField : public EmitterField
{
public:

  RandomRangeEmitterField(size_t dim) : EmitterField(dim) {}
  ~RandomRangeEmitterField() = default;

  RandomRangeEmitterField&
  operator=(const RandomRangeEmitterField& other)               = delete;
  RandomRangeEmitterField(const RandomRangeEmitterField& other) = delete;

  RandomRangeEmitterField& operator=(RandomRangeEmitterField&& other) = delete;
  RandomRangeEmitterField(RandomRangeEmitterField&& other)            = delete;

  template <glm::length_t Dim>
  inline void setMinValue(const glm::vec<Dim, float>& value)
  {
    m_minValue = glm::vec4(value);
  }

  template <glm::length_t Dim>
  inline void setMaxValue(const glm::vec<Dim, float>& value)
  {
    m_maxValue = glm::vec4(value);
  }

  inline const glm::vec4& getMinValue() const { return m_minValue; }
  inline const glm::vec4& getMaxValue() const { return m_maxValue; }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    defines.add(prefix + "_RandomRange");
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_minAttrib = layout.pushVectorFloat(getDim());
    m_maxAttrib = layout.pushVectorFloat(getDim());
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    buffer.setAttribute(std::span{ &m_minValue.x, getDim() }, m_minAttrib, 0);
    buffer.setAttribute(std::span{ &m_maxValue.x, getDim() }, m_maxAttrib, 0);
  }

private:

  glm::vec4 m_minValue;
  glm::vec4 m_maxValue;

  render::SSBO430Layout::Attrib m_minAttrib;
  render::SSBO430Layout::Attrib m_maxAttrib;
};

} // namespace vrm
