#pragma once

#include <array>
#include <glm/glm.hpp>

namespace vrm
{

struct alignas(16) SSBOCluster
{
    glm::vec4 minAABB_VS;
    glm::vec4 maxAABB_VS;
    unsigned int indexCount;
    std::array<unsigned int, 100> lightIndices;

    std::vector<std::pair<const void*, size_t>> getData() const
    {
        return { { &minAABB_VS, sizeof(SSBOCluster) } };
    }
};

} // namespace vrm
