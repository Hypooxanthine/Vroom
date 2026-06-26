#ifndef _COMMONLIGHTS_GLSL_
#define _COMMONLIGHTS_GLSL_

// Maximum number of shadow cascades per directional light. Sizes the u_cascadeSplits
// array of the LightMatricesBlock SSBO. Must stay in sync with
// ShadowMappingPass::kMaxShadowCascades (Vroom/Renderer/private/Renderer/ShadowMappingPass.h).
#define VRM_MAX_SHADOW_CASCADES 5

struct DirectionalLight
{
  mat4 viewProj;
  vec4  direction;
  vec4  color;
  float intensity;
  uint  castsShadows;
};

struct PointLight
{
  vec4  position;
  vec4  color;
  float intensity;
  float radius;
  float smoothRadius;
  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;
};

#endif
