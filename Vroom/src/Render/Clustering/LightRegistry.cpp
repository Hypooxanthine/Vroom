#include "Vroom/Render/Clustering/LightRegistry.h"

namespace vrm
{

void LightRegistry::beginFrame()
{
    m_ShouldResetSSBO = false;
    m_NextFramePointLights.clear();
    m_ChangedPointLights.clear();
    m_RemovedPointLights.clear();

    for (size_t i = 0; i < m_PointLights.size(); ++i)
    {
        m_RemovedPointLights.insert(i);
    }
}
    
void LightRegistry::submitPointLight(const PointLightComponent& pointLight, const glm::vec3& position, const std::string& identifier)
{
    m_NextFramePointLightMap[identifier] = m_NextFramePointLights.size();
    m_NextFramePointLights.push_back({ position, pointLight.color, pointLight.intensity, pointLight.radius });

    auto it = m_PointLightMap.find(identifier);
    if (it == m_PointLightMap.end())
        // We are adding a new light to the registry. The SSBO will need to be reset to store the new light.
        m_ShouldResetSSBO = true;
    else
        // A light has been updated, so we need to remove it from the removed set.
        m_RemovedPointLights.erase(it->second);
}

void LightRegistry::endFrame()
{
    if (m_RemovedPointLights.size() > 0)
        m_ShouldResetSSBO = true;

    m_PointLights = m_NextFramePointLights;
    m_PointLightMap = m_NextFramePointLightMap;
}

const SSBOPointLightData* LightRegistry::getPointLightData() const
{
    return m_PointLights.data();
}

const std::vector<SSBOPointLightData>& LightRegistry::getPointLights() const
{
    return m_PointLights;
}

size_t LightRegistry::getPointLightCount() const
{
    return m_PointLights.size();
}

bool LightRegistry::shouldResetSSBO() const
{
    return m_ShouldResetSSBO;
}

const std::list<size_t>& LightRegistry::getChangedPointLights() const
{
    return m_ChangedPointLights;
}

} // namespace vrm
