#include "Renderer/LightRegistry.h"

#include "RenderObjects/DirectionalLight.h"
#include "RenderObjects/PointLight.h"

using namespace vrm;

LightRegistry::LightRegistry()
{}

void LightRegistry::submitLight(const render::PointLight& pointLight, size_t identifier)
{
  const RawPointLight* retrievedLight = m_pointLightsRegistry.tryGetElement(identifier);

  RawPointLight raw;
  raw.position             = glm::vec4(pointLight.position, 1.f);
  raw.color                = glm::vec4(pointLight.color, 1.f);
  raw.intensity            = pointLight.intensity;
  raw.radius               = pointLight.radius;
  raw.smoothRadius         = pointLight.smoothRadius;
  raw.constantAttenuation  = pointLight.constantAttenuation;
  raw.linearAttenuation    = pointLight.linearAttenuation;
  raw.quadraticAttenuation = pointLight.quadraticAttenuation;

  m_pointLightsRegistry.submit(identifier, raw);
}

void LightRegistry::submitLight(const render::DirectionalLight& dirLight, size_t identifier)
{
  RawDirLight raw;
  raw.direction    = glm::vec4(dirLight.direction, 1.f);
  raw.color        = glm::vec4(dirLight.color, 1.f);
  raw.intensity    = dirLight.intensity;
  raw.castsShadows = dirLight.castsShadows;

  m_dirLightsRegistry.submit(identifier, raw);
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
  if (!m_dirLightsRegistry.wasJustModified())
    return;

  DirLightRegistry&   registry = m_dirLightsRegistry;
  render::AutoBuffer& buffer   = m_dirLightsBuffer;

  // std430 rule : the first array element is aligned on the size of its biggest
  // attribute (here, vec4)
  size_t headerSizeBytes = sizeof(glm::vec4);
  size_t lightsSizeBytes = registry.getElementCount() * sizeof(DirLightRegistry::ElementType);
  buffer.ensureCapacity(headerSizeBytes + lightsSizeBytes);

  {
    std::span<uint8_t> map = buffer.mapWriteOnly();

    glm::uint* header = reinterpret_cast<glm::uint*>(map.data());
    *header           = static_cast<glm::uint>(registry.getElementCount());

    RawDirLight* lights = reinterpret_cast<RawDirLight*>(map.data() + headerSizeBytes);
    std::memcpy(lights, registry.getRawData(), lightsSizeBytes);

    buffer.unmap();
  }
}

void LightRegistry::_updateGpuPointLights()
{
  if (!m_pointLightsRegistry.wasJustModified())
    return;

  PointLightRegistry& registry = m_pointLightsRegistry;
  render::AutoBuffer& buffer   = m_pointLightsBuffer;

  // std430 rule : the first array element is aligned on the size of its biggest
  // attribute (here, vec4)
  size_t headerSizeBytes = sizeof(glm::vec4);
  size_t lightsSizeBytes = registry.getElementCount() * sizeof(PointLightRegistry::ElementType);
  buffer.ensureCapacity(headerSizeBytes + lightsSizeBytes);

  {
    std::span<uint8_t> map = buffer.mapWriteOnly();

    uint32_t* header = reinterpret_cast<uint32_t*>(map.data());
    *header          = static_cast<uint32_t>(registry.getElementCount());

    RawPointLight* lights = reinterpret_cast<RawPointLight*>(map.data() + headerSizeBytes);
    std::memcpy(lights, registry.getRawData(), lightsSizeBytes);

    buffer.unmap();
  }
}
