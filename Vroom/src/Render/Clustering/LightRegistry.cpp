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
      .position = position,
      .color = pointLight.color, 
      .intensity = pointLight.intensity,
      .radius = pointLight.radius
    }
  );
}

void LightRegistry::submitLight(const DirectionalLightComponent &dirLight, const glm::vec3 &direction, size_t identifier)
{
  m_dirLightsRegistry.submit(
    identifier,
    SSBODirectionalLightData {
      .direction = direction,
      .color = dirLight.color,
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
