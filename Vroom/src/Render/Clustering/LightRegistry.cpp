#include "Vroom/Render/Clustering/LightRegistry.h"

namespace vrm
{

void LightRegistry::setBindingPoint(int bindingPoint)
{
    m_SSBOPointLights.setBindingPoint(bindingPoint);
}

void LightRegistry::reserve(int lightCount)
{
    m_SSBOPointLights.reserve(sizeof(int) + lightCount * sizeof(SSBOPointLightData));
}

void LightRegistry::beginFrame()
{
    m_AddedPointLights.clear();

    // At the beginning of the frame, every address is set to free.
    // If a light is added, it will remove the address from the free addresses set.
    for (const auto& [_, address] : m_PointLightAddresses)
        m_FreePointLightAdresses.insert(address);
}

void LightRegistry::submitPointLight(const PointLightComponent& pointLight, const glm::vec3& position, const std::string& identifier)
{
    SSBOPointLightData pointLightData{position, pointLight.color, pointLight.intensity, pointLight.radius};

    if (auto it = m_PointLightAddresses.find(identifier); it != m_PointLightAddresses.end()) // Light already exists
    {
        const auto& address = it->second;
        // Mark the address of the light as not free
        m_FreePointLightAdresses.erase(address);

        // Update the light
        m_SSBOPointLights.setSubData(&pointLightData, sizeof(SSBOPointLightData), address);
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
    int toReserve = static_cast<int>(m_PointLightAddresses.size() + m_AddedPointLights.size() - m_FreePointLightAdresses.size());

    if (toReserve > 0)
        reserve(toReserve);

    for (const auto& [id, pointLight] : m_AddedPointLights)
    {
        if (auto it = m_FreePointLightAdresses.begin(); it != m_FreePointLightAdresses.end())
        {
            // We can reuse the memory of a previously removed light
            const int& freeAddress = *it;
            m_SSBOPointLights.setSubData(&pointLight, sizeof(SSBOPointLightData), freeAddress);
            m_PointLightAddresses[id] = freeAddress;
            m_FreePointLightAdresses.erase(it);
        }
        else
        {
            // We need to add some new data to the SSBO
            int address = m_SSBOPointLights.getSize();
            m_SSBOPointLights.setSubData(&pointLight, sizeof(SSBOPointLightData), address);
            m_PointLightAddresses[id] = address;
        }
    }
}

} // namespace vrm
