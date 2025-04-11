#pragma once

#include <glm/glm.hpp>

namespace vrm
{

/**
 * @brief Data for a point light to be stored in a SSBO. This is the formatted data for openGL std430 SSBO.
 * In the shader, we'll need to retrieve vec3's with float[3]'s, because of the std430 layout.
 * 
 */
struct alignas(16) SSBOPointLightData
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;
};

} // namespace vrm
