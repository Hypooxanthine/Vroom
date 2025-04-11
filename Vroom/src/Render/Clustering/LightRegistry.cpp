#include "Vroom/Render/Clustering/LightRegistry.h"

using namespace vrm;

LightRegistry::LightRegistry()
{
  m_ssbo.reset(s_ssboOffset);
  updateGpuSize();
}

void LightRegistry::submitPointLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier)
{
  VRM_ASSERT_MSG(!m_idToAddress.contains(identifier), "Pointlight already registered");

  size_t addr = getValidAddress();
  m_idToAddress[identifier] = addr;
  m_activeLights.at(addr) = true;

  updatePointLight(pointLight, position, identifier);

  markDirty();
}

void LightRegistry::updatePointLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier)
{
  auto &pl = getPointLight(identifier);
  pl.color = pointLight.color;
  pl.intensity = pointLight.intensity;
  pl.radius = pointLight.radius;
  pl.position = position;
}

void LightRegistry::removePointLight(size_t identifier)
{
  VRM_ASSERT_MSG(m_idToAddress.contains(identifier), "Pointlight not registered");

  size_t freed = m_idToAddress.at(identifier);

  m_idToAddress.erase(identifier);
  m_freeAddresses.emplace(freed);
  m_activeLights.at(freed) = false;

  markDirty();
}

void LightRegistry::update()
{
  if (isDirty())
  {
    // Lights compaction
    std::vector<SSBOPointLightData> newData;
    newData.reserve(m_lights.size() - m_freeAddresses.size());
    std::unordered_map<size_t, size_t> remap;

    for (size_t i = 0; i < m_activeLights.size(); ++i)
    {
      if (m_activeLights.at(i))
      {
        remap[i] = newData.size();
        newData.emplace_back(m_lights.at(i));
      }
    }

    for (auto& [id, addr] : m_idToAddress)
    {
      addr = remap.at(addr);
    }

    m_lights = std::move(newData);
    m_activeLights.assign(m_lights.size(), true); // All lights are active now
    m_freeAddresses.clear();

    m_dirty = false;
  }

  // Size updating
  updateGpuSize();

  // Lights updating
  m_ssbo.setData(m_lights.data(), s_pointLightSize * static_cast<GLsizei>(m_lights.size()), s_ssboOffset);
}

size_t LightRegistry::getValidAddress()
{
  size_t out;
  if (m_freeAddresses.empty())
  {
    out = m_lights.size();
    m_lights.emplace_back();
    m_activeLights.emplace_back();
  }
  else
  {
    auto it = m_freeAddresses.begin();
    out = *it;
    m_freeAddresses.erase(it);
  }

  return out;
}

void LightRegistry::updateGpuSize()
{
  uint32_t lightCount = static_cast<uint32_t>(m_lights.size());
  m_ssbo.setData(&lightCount, sizeof(uint32_t), 0);
}

SSBOPointLightData& LightRegistry::getPointLight(size_t identifier)
{
  VRM_ASSERT_MSG(m_idToAddress.contains(identifier), "Pointlight not registered");

  return m_lights.at(m_idToAddress.at(identifier));
}
