#pragma once

namespace vrm
{

class RawParticleEmitterSpecs
{
public:

  enum class EFieldType
  {
    eConst = 0,
    eRandomRange
  };

  struct Layout
  {
    EFieldType colorSpawn, colorDeath;
    EFieldType positionSpawn, positionDeath;
    EFieldType scaleSpawn, scaleDeath;
  };

public:

  RawParticleEmitterSpecs();
  ~RawParticleEmitterSpecs();

  RawParticleEmitterSpecs&
  operator=(const RawParticleEmitterSpecs& other)               = delete;
  RawParticleEmitterSpecs(const RawParticleEmitterSpecs& other) = delete;

  RawParticleEmitterSpecs& operator=(RawParticleEmitterSpecs&& other) = delete;
  RawParticleEmitterSpecs(RawParticleEmitterSpecs&& other)            = delete;

  void setupLayout();

private:

  void _clear();

private:

  char*  m_data     = nullptr;
  size_t m_dataSize = 0;
};

} // namespace vrm
