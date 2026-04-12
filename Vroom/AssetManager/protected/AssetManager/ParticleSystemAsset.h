#pragma once

#include "AssetManager/ParticleSystemData.h"
#include "AssetManager/AssetHandle.h"
#include "AssetManager/StaticAsset.h"
namespace vrm
{

  class ParticleSystemAsset : public StaticAsset
  {
  public:

    using Handle = AssetHandle<ParticleSystemAsset>;

  public:

    ParticleSystemAsset();
    ~ParticleSystemAsset();

    ParticleSystemAsset& operator=(const ParticleSystemAsset& other) = delete;
    ParticleSystemAsset(const ParticleSystemAsset& other) = delete;

    ParticleSystemAsset& operator=(ParticleSystemAsset&& other) = delete;
    ParticleSystemAsset(ParticleSystemAsset&& other) = delete;

    inline const ParticleSystemData& getData() const { return m_data; }

  protected:

    bool loadImpl(const std::string &filePath) override;

  private:

    ParticleSystemData m_data;

  };

}