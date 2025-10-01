#pragma once

#include <string>
#include <variant>

#include <glm/glm.hpp>

#include "Vroom/Render/MaterialDefines.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "Vroom/Render/SSBO430Layout.h"
#include "Vroom/Render/StructuredBuffer.h"

namespace vrm
{

using ParticleEmitterFieldType =
  std::variant<ConstParticleEmitterField, RandomRangeParticleEmitterField>;

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
        || deathValue.index() != other.deathValue.index();
  }

  const std::string& getShaderDefineName() const { return m_shaderDefineName; }

private:

  // No VRM_ prefix, no trailing/tailing underscores
  std::string m_shaderDefineName;
};

class EmitterAttrib
{
public:

  EmitterAttrib()  = default;
  ~EmitterAttrib() = default;

  EmitterAttrib& operator=(const EmitterAttrib& other) = delete;
  EmitterAttrib(const EmitterAttrib& other)            = delete;

  EmitterAttrib& operator=(EmitterAttrib&& other) = delete;
  EmitterAttrib(EmitterAttrib&& other)            = delete;

  virtual void applyDefines(MaterialDefines&   defines,
                            const std::string& prefix)                      = 0;
  virtual void pushToLayout(render::SSBO430Layout& layout)                  = 0;
  virtual void assignStructuredBufferData(render::StructuredBuffer& buffer) = 0;

private:
};

class SetSpawnVelocityEmitterAttrib : public EmitterAttrib
{
public:

  inline SetSpawnVelocityEmitterAttrib(EmitterFieldPtr&& velocity)
    : m_velocity(std::move(velocity))
  {}

  inline SetSpawnVelocityEmitterAttrib()
    : SetSpawnVelocityEmitterAttrib(std::make_unique<ConstEmitterField>(3))
  {}

  ~SetSpawnVelocityEmitterAttrib() = default;

  SetSpawnVelocityEmitterAttrib&
  operator=(const SetSpawnVelocityEmitterAttrib& other) = delete;
  SetSpawnVelocityEmitterAttrib(const SetSpawnVelocityEmitterAttrib& other) =
    delete;

  SetSpawnVelocityEmitterAttrib&
  operator=(SetSpawnVelocityEmitterAttrib&& other)                     = delete;
  SetSpawnVelocityEmitterAttrib(SetSpawnVelocityEmitterAttrib&& other) = delete;

  inline void setVelocity(EmitterFieldPtr&& field)
  {
    m_velocity = std::move(field);
  }

  inline const EmitterField& getVelocity() const { return *m_velocity; }

  void applyDefines(MaterialDefines&   defines,
                    const std::string& prefix) override
  {
    m_velocity->applyDefines(defines, prefix + "SetSpawnVelocity");
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

} // namespace vrm
