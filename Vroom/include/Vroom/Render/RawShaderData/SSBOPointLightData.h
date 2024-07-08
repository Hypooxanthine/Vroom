#pragma once

#include <glm/glm.hpp>

namespace vrm
{

/**
 * @brief Data for a point light to be stored in a SSBO. This is the formatted data for openGL std430 SSBO.
 * In the shader, we'll need to retrieve vec3's with float[3]'s, because of the std430 layout.
 * 
 */
struct SSBOPointLightData
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float radius;

    bool operator==(const SSBOPointLightData& other) const
    {
        return position == other.position && color == other.color && intensity == other.intensity && radius == other.radius;
    }

    bool operator!=(const SSBOPointLightData& other) const
    {
        return !(*this == other);
    }

    std::vector<std::pair<const void*, size_t>> getData() const
    {
        return { { &position, sizeof(glm::vec3) * 2 + sizeof(float) * 2 } };
    }
};

} // namespace vrm
