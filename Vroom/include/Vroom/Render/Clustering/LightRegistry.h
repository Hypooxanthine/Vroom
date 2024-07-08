#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>

#include <glm/glm.hpp>

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"
#include "Vroom/Render/Abstraction/DynamicSSBO.h"

namespace vrm
{

class LightRegistry
{
public:
    LightRegistry() = default;
    LightRegistry(const LightRegistry&) = default;
    LightRegistry(LightRegistry&&) = default;
    ~LightRegistry() = default;

    LightRegistry& operator=(const LightRegistry&) = default;
    LightRegistry& operator=(LightRegistry&&) = default;

    void setBindingPoint(int bindingPoint);
    void reserve(int lightCount);

    void beginFrame();

    void submitPointLight(const PointLightComponent& pointLight, const glm::vec3& position, const std::string& identifier);

    void endFrame();

    const std::unordered_map<int, SSBOPointLightData>& getPointLights() const { return m_PointLights; }

private:
    void updateData();

private:
    // Point lights that are currently in the scene
    std::unordered_map<int, SSBOPointLightData> m_PointLights;
    // SSBO for point lights
    DynamicSSBO m_SSBOPointLights;
    // Identifiers point to the address of the light in the SSBO
    std::unordered_map<std::string, int> m_PointLightAddresses;

    std::unordered_map<std::string, SSBOPointLightData> m_AddedPointLights;
    std::unordered_set<int> m_FreePointLightAdresses;
    int m_NextPointLightAddress = sizeof(int);
};

} // namespace vrm
