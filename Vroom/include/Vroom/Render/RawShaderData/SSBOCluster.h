#pragma once

namespace vrm
{

struct SSBOCluster
{
    int indexCount;
    int indexOffset;

    std::vector<std::pair<const void*, size_t>> getData() const
    {
        return { { &indexCount, sizeof(int) * 2 } };
    }
};

} // namespace vrm
