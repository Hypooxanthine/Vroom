#pragma once

#include <vector>

namespace vrm
{

struct SSBOLightIndices
{
    std::vector<int> indices;

    std::vector<std::pair<const void*, size_t>> getData() const
    {
        return { { indices.data(), sizeof(int) * indices.size() } };
    }
};

} // namespace vrm
