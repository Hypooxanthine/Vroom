#include "Vroom/Render/Clustering/LightRegistry.h"

namespace vrm
{

  LightRegistry::LightRegistry()
  {
    m_SSBOPointLights.reset(0);
  }

  void LightRegistry::beginFrame()
  {
    m_AddedPointLights.clear();
    m_PointLights.clear();

    // At the beginning of the frame, every address is set to free.
    // If a light is added, it will remove the address from the free addresses set.
    for (const auto &[_, address] : m_PointLightAddresses)
      m_FreePointLightAdresses.insert(address);
  }

  void LightRegistry::submitPointLight(const PointLightComponent &pointLight, const glm::vec3 &position, const std::string &identifier)
  {
    SSBOPointLightData pointLightData{position, pointLight.color, pointLight.intensity, pointLight.radius};

    if (auto it = m_PointLightAddresses.find(identifier); it != m_PointLightAddresses.end()) // Light already exists
    {
      const auto &address = it->second;
      // Mark the address of the light as not free
      m_FreePointLightAdresses.erase(address);

      // Update the light
      // m_SSBOPointLights.setSubData(&pointLightData, sizeof(SSBOPointLightData), address);
      m_SSBOPointLights.setData(&pointLightData, sizeof(SSBOPointLightData), address);
      int lightIndex = (address - sizeof(int)) / sizeof(SSBOPointLightData);
      m_PointLights[lightIndex] = pointLightData;
    }
    else // Light doesn't exist
    {
      // At the end of the frame, the free addresses set will be relevant, and we'll be able to reuse the memory of removed lights
      // for new lights.
      m_AddedPointLights[identifier] = pointLightData;
    }
  }

  void LightRegistry::endFrame()
  {
    updateData();
  }

  void LightRegistry::updateData()
  {
    for (const auto &[id, pointLight] : m_AddedPointLights)
    {
      if (auto it = m_FreePointLightAdresses.begin(); it != m_FreePointLightAdresses.end())
      {
        // We can reuse the memory of a previously removed light
        const int &freeAddress = *it;
        m_SSBOPointLights.setData(&pointLight, sizeof(SSBOPointLightData), freeAddress);
        m_PointLightAddresses[id] = freeAddress;
        m_FreePointLightAdresses.erase(it);
        int recycledAdress = (freeAddress - sizeof(int)) / sizeof(SSBOPointLightData);
        m_PointLights[recycledAdress] = pointLight;
      }
      else
      {
        // We need to add some new data to the SSBO
        m_SSBOPointLights.setData(&pointLight, sizeof(SSBOPointLightData), m_NextPointLightAddress);
        m_PointLightAddresses[id] = m_NextPointLightAddress;
        int index = (m_NextPointLightAddress - sizeof(int)) / sizeof(SSBOPointLightData);
        m_PointLights[index] = pointLight;
        m_NextPointLightAddress += sizeof(SSBOPointLightData);
      }
    }

    int lightCount = static_cast<int>(m_PointLights.size());
    m_SSBOPointLights.setData(&lightCount, sizeof(int), 0);

    // VRM_LOG_TRACE("SSBO size: {0}", m_SSBOPointLights.getCapacity());
    // VRM_LOG_TRACE("Lights count in registry: {0}", m_PointLights.size());
  }

} // namespace vrm
