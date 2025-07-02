#include "Vroom/Render/Clustering/LightRegistry.h"

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"

using namespace vrm;

LightRegistry::LightRegistry()
  : m_dirLightsRegistry(&m_ssbo, 0), m_pointLightsRegistry(&m_ssbo, s_pointLightOffset)
{
  m_ssbo.reset(s_pointLightOffset + sizeof(uint32_t));
}

void LightRegistry::submitLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier)
{
  m_pointLightsRegistry.submit(
    identifier,
    SSBOPointLightData {
      .position = glm::vec4(position, 1.f),
      .color = glm::vec4(pointLight.color, 1.f),
      .intensity = pointLight.intensity,
      .radius = pointLight.radius,
      .constantAttenuation = pointLight.constantAttenuation,
      .linearAttenuation = pointLight.linearAttenuation,
      .quadraticAttenuation = pointLight.quadraticAttenuation,
    }
  );
}

void LightRegistry::submitLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier)
{
  m_dirLightsRegistry.submit(
    identifier,
    SSBODirectionalLightData {
      .direction = glm::vec4(direction, 1.f),
      .color = glm::vec4(dirLight.color, 1.f),
      .intensity = dirLight.intensity,
      .castsShadows = dirLight.castsShadows
    }
  );
}

void LightRegistry::startRegistering()
{
  m_dirLightsRegistry.startRegistering();
  m_pointLightsRegistry.startRegistering();
}

void LightRegistry::endRegistering()
{
  m_dirLightsRegistry.endRegistering();
  m_pointLightsRegistry.endRegistering();
}
