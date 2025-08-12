#pragma once

namespace vrm
{

  class ParticleSystemData
  {
  public:

    ParticleSystemData() = default;
    ~ParticleSystemData() = default;

    ParticleSystemData& operator=(const ParticleSystemData& other) = delete;
    ParticleSystemData(const ParticleSystemData& other) = delete;

    ParticleSystemData& operator=(ParticleSystemData&& other) = delete;
    ParticleSystemData(ParticleSystemData&& other) = delete;

  private:

  
  };

}