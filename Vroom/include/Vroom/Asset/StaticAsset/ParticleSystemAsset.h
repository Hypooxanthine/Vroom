#pragma once

#include "Vroom/Asset/AssetData/ParticleSystemData.h"
#include "Vroom/Asset/AssetHandle.h"
#include "Vroom/Asset/StaticAsset/StaticAsset.h"
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