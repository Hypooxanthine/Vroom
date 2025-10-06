#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"

namespace vrm
{

enum class EmitterAttribType
{
  LifeTime = 0,
  EmitRate,
  SpawnPosition,
  SpawnVelocity,
  SpawnColor,
  SpawnScale
};

class IEmitterAttrib
{
public:

  virtual IEmitterAttrib* clone() const                                     = 0;
  virtual bool structuresDifferent(const IEmitterAttrib& other) const       = 0;
  virtual EmitterAttribType getType() const                                 = 0;
  virtual void              applyDefines(MaterialDefines&   defines,
                                         const std::string& prefix)         = 0;
  virtual void              pushToLayout(render::SSBO430Layout& layout)     = 0;
  virtual void assignStructuredBufferData(render::StructuredBuffer& buffer) = 0;
};

class SpawnPositionEmitterAttrib : public IEmitterAttrib
{
public:

  inline SpawnPositionEmitterAttrib(EmitterFieldPtr&& position)
    : m_position(std::move(position))
  {}

  inline SpawnPositionEmitterAttrib()
    : SpawnPositionEmitterAttrib(std::make_unique<ConstEmitterField3>())
  {}

  ~SpawnPositionEmitterAttrib() = default;

  SpawnPositionEmitterAttrib&
  operator=(const SpawnPositionEmitterAttrib& other)                  = delete;
  SpawnPositionEmitterAttrib(const SpawnPositionEmitterAttrib& other) = delete;

  SpawnPositionEmitterAttrib&
  operator=(SpawnPositionEmitterAttrib&& other)                  = delete;
  SpawnPositionEmitterAttrib(SpawnPositionEmitterAttrib&& other) = delete;

  inline void setPosition(EmitterFieldPtr&& field)
  {
    m_position = std::move(field);
  }

  inline EmitterFieldPtr&     getPositionFieldPtr() { return m_position; }
  inline const IEmitterField& getPositionField() const { return *m_position; }
  inline IEmitterField&       getPositionField() { return *m_position; }

  SpawnPositionEmitterAttrib* clone() const override
  {
    EmitterFieldPtr positionField(
      static_cast<IEmitterField*>(m_position->clone()));

    return new SpawnPositionEmitterAttrib(std::move(positionField));
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    return getType() != other.getType()
        || m_position->structureDifferent(
          *static_cast<const SpawnPositionEmitterAttrib&>(other).m_position);
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::SpawnPosition;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    static const std::string attribSuffix = "_SpawnPosition";
    defines.add(prefix + "_USE" + attribSuffix);
    m_position->applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_position->pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_position->assignStructuredBufferData(buffer);
  }

private:

  EmitterFieldPtr m_position;
};

class LifeTimeEmitterAttrib : public IEmitterAttrib
{
public:

  inline LifeTimeEmitterAttrib(EmitterFieldPtr&& lifeTime)
    : m_lifeTime(std::move(lifeTime))
  {}

  inline LifeTimeEmitterAttrib()
    : LifeTimeEmitterAttrib(std::make_unique<ConstEmitterField1>())
  {}

  ~LifeTimeEmitterAttrib() = default;

  LifeTimeEmitterAttrib& operator=(const LifeTimeEmitterAttrib& other) = delete;
  LifeTimeEmitterAttrib(const LifeTimeEmitterAttrib& other)            = delete;

  LifeTimeEmitterAttrib& operator=(LifeTimeEmitterAttrib&& other) = delete;
  LifeTimeEmitterAttrib(LifeTimeEmitterAttrib&& other)            = delete;

  inline void setLifeTime(EmitterFieldPtr&& field)
  {
    m_lifeTime = std::move(field);
  }

  inline EmitterFieldPtr&     getLifeTimeFieldPtr() { return m_lifeTime; }
  inline const IEmitterField& getLifeTimeField() const { return *m_lifeTime; }
  inline IEmitterField&       getLifeTimeField() { return *m_lifeTime; }

  inline float getUpperBound() const { return m_lifeTime->getUpperBound().x; }

  LifeTimeEmitterAttrib* clone() const override
  {
    EmitterFieldPtr lifeTimeField(
      static_cast<IEmitterField*>(m_lifeTime->clone()));

    return new LifeTimeEmitterAttrib(std::move(lifeTimeField));
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    return getType() != other.getType()
        || m_lifeTime->structureDifferent(
          *static_cast<const LifeTimeEmitterAttrib&>(other).m_lifeTime);
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::LifeTime;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    static const std::string attribSuffix = "_LifeTime";
    defines.add(prefix + "_USE" + attribSuffix);
    m_lifeTime->applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_lifeTime->pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_lifeTime->assignStructuredBufferData(buffer);
  }

private:

  EmitterFieldPtr m_lifeTime;
};

/**
 * @brief Only const field is supported
 *
 */
class EmitRateEmitterAttrib : public IEmitterAttrib
{
public:

  inline EmitRateEmitterAttrib(float emitRate) { setEmitRate(emitRate); }

  inline EmitRateEmitterAttrib(ConstEmitterField1 emitRate)
    : m_emitRate(emitRate)
  {}

  inline EmitRateEmitterAttrib() : EmitRateEmitterAttrib(1.f) {}

  ~EmitRateEmitterAttrib() = default;

  EmitRateEmitterAttrib& operator=(const EmitRateEmitterAttrib& other) = delete;
  EmitRateEmitterAttrib(const EmitRateEmitterAttrib& other)            = delete;

  EmitRateEmitterAttrib& operator=(EmitRateEmitterAttrib&& other) = delete;
  EmitRateEmitterAttrib(EmitRateEmitterAttrib&& other)            = delete;

  inline void setEmitRate(float emitRate)
  {
    m_emitRate.setValue(glm::vec1(emitRate));
  }

  inline float getEmitRate() const { return m_emitRate.getValue().x; }

  EmitRateEmitterAttrib* clone() const override
  {
    return new EmitRateEmitterAttrib(m_emitRate);
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    // For now emit rate can only be a constant float
    return getType() != other.getType();
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::EmitRate;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    // It has to be a const float anyway, but to stay consistent
    static const std::string attribSuffix = "_EmitRate";
    defines.add(prefix + "_USE" + attribSuffix);
    m_emitRate.applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_emitRate.pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_emitRate.assignStructuredBufferData(buffer);
  }

private:

  ConstEmitterField1 m_emitRate;
};

class SpawnVelocityEmitterAttrib : public IEmitterAttrib
{
public:

  inline SpawnVelocityEmitterAttrib(EmitterFieldPtr&& velocity)
    : m_velocity(std::move(velocity))
  {}

  inline SpawnVelocityEmitterAttrib()
    : SpawnVelocityEmitterAttrib(std::make_unique<ConstEmitterField3>())
  {}

  ~SpawnVelocityEmitterAttrib() = default;

  SpawnVelocityEmitterAttrib&
  operator=(const SpawnVelocityEmitterAttrib& other)                  = delete;
  SpawnVelocityEmitterAttrib(const SpawnVelocityEmitterAttrib& other) = delete;

  SpawnVelocityEmitterAttrib&
  operator=(SpawnVelocityEmitterAttrib&& other)                  = delete;
  SpawnVelocityEmitterAttrib(SpawnVelocityEmitterAttrib&& other) = delete;

  inline void setVelocity(EmitterFieldPtr&& field)
  {
    m_velocity = std::move(field);
  }

  inline EmitterFieldPtr&     getVelocityFieldPtr() { return m_velocity; }
  inline const IEmitterField& getVelocityField() const { return *m_velocity; }
  inline IEmitterField&       getVelocityField() { return *m_velocity; }

  SpawnVelocityEmitterAttrib* clone() const override
  {
    EmitterFieldPtr velocityField(
      static_cast<IEmitterField*>(m_velocity->clone()));

    return new SpawnVelocityEmitterAttrib(std::move(velocityField));
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    return getType() != other.getType()
        || m_velocity->structureDifferent(
          *static_cast<const SpawnVelocityEmitterAttrib&>(other).m_velocity);
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::SpawnVelocity;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    static const std::string attribSuffix = "_SpawnVelocity";
    defines.add(prefix + "_USE" + attribSuffix);
    m_velocity->applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_velocity->pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_velocity->assignStructuredBufferData(buffer);
  }

private:

  EmitterFieldPtr m_velocity;
};

class SpawnColorEmitterAttrib : public IEmitterAttrib
{
public:

  inline SpawnColorEmitterAttrib(EmitterFieldPtr&& color)
    : m_color(std::move(color))
  {}

  inline SpawnColorEmitterAttrib()
    : SpawnColorEmitterAttrib(std::make_unique<ConstEmitterField4>())
  {}

  ~SpawnColorEmitterAttrib() = default;

  SpawnColorEmitterAttrib&
  operator=(const SpawnColorEmitterAttrib& other)               = delete;
  SpawnColorEmitterAttrib(const SpawnColorEmitterAttrib& other) = delete;

  SpawnColorEmitterAttrib& operator=(SpawnColorEmitterAttrib&& other) = delete;
  SpawnColorEmitterAttrib(SpawnColorEmitterAttrib&& other)            = delete;

  inline void setColor(EmitterFieldPtr&& field) { m_color = std::move(field); }

  inline EmitterFieldPtr&     getColorFieldPtr() { return m_color; }
  inline const IEmitterField& getColorField() const { return *m_color; }
  inline IEmitterField&       getColorField() { return *m_color; }

  SpawnColorEmitterAttrib* clone() const override
  {
    EmitterFieldPtr colorField(static_cast<IEmitterField*>(m_color->clone()));

    return new SpawnColorEmitterAttrib(std::move(colorField));
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    return getType() != other.getType()
        || m_color->structureDifferent(
          *static_cast<const SpawnColorEmitterAttrib&>(other).m_color);
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::SpawnColor;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    static const std::string attribSuffix = "_SpawnColor";
    defines.add(prefix + "_USE" + attribSuffix);
    m_color->applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_color->pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_color->assignStructuredBufferData(buffer);
  }

private:

  EmitterFieldPtr m_color;
};

class SpawnScaleEmitterAttrib : public IEmitterAttrib
{
public:

  inline SpawnScaleEmitterAttrib(EmitterFieldPtr&& scale)
    : m_scale(std::move(scale))
  {}

  inline SpawnScaleEmitterAttrib()
    : SpawnScaleEmitterAttrib(std::make_unique<ConstEmitterField3>())
  {}

  ~SpawnScaleEmitterAttrib() = default;

  SpawnScaleEmitterAttrib&
  operator=(const SpawnScaleEmitterAttrib& other)               = delete;
  SpawnScaleEmitterAttrib(const SpawnScaleEmitterAttrib& other) = delete;

  SpawnScaleEmitterAttrib& operator=(SpawnScaleEmitterAttrib&& other) = delete;
  SpawnScaleEmitterAttrib(SpawnScaleEmitterAttrib&& other)            = delete;

  inline void setScale(EmitterFieldPtr&& field) { m_scale = std::move(field); }

  inline EmitterFieldPtr&     getScaleFieldPtr() { return m_scale; }
  inline const IEmitterField& getScaleField() const { return *m_scale; }
  inline IEmitterField&       getScaleField() { return *m_scale; }

  SpawnScaleEmitterAttrib* clone() const override
  {
    EmitterFieldPtr scaleField(static_cast<IEmitterField*>(m_scale->clone()));

    return new SpawnScaleEmitterAttrib(std::move(scaleField));
  }

  bool structuresDifferent(const IEmitterAttrib& other) const override
  {
    return getType() != other.getType()
        || m_scale->structureDifferent(
          *static_cast<const SpawnScaleEmitterAttrib&>(other).m_scale);
  }

  EmitterAttribType getType() const override
  {
    return EmitterAttribType::SpawnScale;
  }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    static const std::string attribSuffix = "_SpawnScale";
    defines.add(prefix + "_USE" + attribSuffix);
    m_scale->applyDefines(defines, prefix + attribSuffix);
  }

  void pushToLayout(render::SSBO430Layout& layout) override
  {
    m_scale->pushToLayout(layout);
  }

  void assignStructuredBufferData(render::StructuredBuffer& buffer) override
  {
    m_scale->assignStructuredBufferData(buffer);
  }

private:

  EmitterFieldPtr m_scale;
};

} // namespace vrm
