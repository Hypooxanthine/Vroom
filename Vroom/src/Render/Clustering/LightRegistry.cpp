#include "Vroom/Render/Clustering/LightRegistry.h"

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"

using namespace vrm;

LightRegistry::LightRegistry()
{
  
}

void LightRegistry::submitLight(const PointLightComponent &pointLight, const glm::vec3 &position, size_t identifier)
{
  m_pointLightsRegistry.submit(
    identifier,
    RawPointLight {
      .position = glm::vec4(position, 1.f),
      .color = glm::vec4(pointLight.color, 1.f),
      .intensity = pointLight.intensity,
      .radius = pointLight.radius,
      .smoothRadius = pointLight.smoothRadius,
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
    RawDirLight {
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

  _updateGpuDirLights();
  _updateGpuPointLights();
}

void LightRegistry::_updateGpuDirLights()
{
  DirLightRegistry& registry = m_dirLightsRegistry;
  render::AutoBuffer& buffer = m_dirLightsBuffer;

  // std430 rule : the first array element is aligned on the size of its biggest
  // attribute (here, vec4)
  size_t headerSizeBytes = sizeof(glm::vec4);
  size_t lightsSizeBytes = registry.getElementCount() * sizeof(DirLightRegistry::ElementType);
  buffer.ensureCapacity(headerSizeBytes + lightsSizeBytes);

  {
    std::span<uint8_t> map = buffer.mapWriteOnly();

    glm::uint* header = reinterpret_cast<glm::uint*>(map.data());
    *header = static_cast<glm::uint>(registry.getElementCount());

    RawDirLight* lights = reinterpret_cast<RawDirLight*>(map.data() + headerSizeBytes);
    std::memcpy(lights, registry.getRawData(), lightsSizeBytes);

    buffer.unmap();
  }
}

void LightRegistry::_updateGpuPointLights()
{
  PointLightRegistry& registry = m_pointLightsRegistry;
  render::AutoBuffer& buffer = m_pointLightsBuffer;

  // std430 rule : the first array element is aligned on the size of its biggest
  // attribute (here, vec4)
  size_t headerSizeBytes = sizeof(glm::vec4);
  size_t lightsSizeBytes = registry.getElementCount() * sizeof(PointLightRegistry::ElementType);
  buffer.ensureCapacity(headerSizeBytes + lightsSizeBytes);

  {
    std::span<uint8_t> map = buffer.mapWriteOnly();

    uint32_t* header = reinterpret_cast<uint32_t*>(map.data());
    *header = static_cast<uint32_t>(registry.getElementCount());

    RawPointLight* lights = reinterpret_cast<RawPointLight*>(map.data() + headerSizeBytes);
    std::memcpy(lights, registry.getRawData(), lightsSizeBytes);

    buffer.unmap();
  }
}
