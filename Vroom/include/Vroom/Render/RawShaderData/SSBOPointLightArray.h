#pragma once

#include <vector>

#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"

namespace vrm
{

struct SSBOPointLightArray
{
    using LightType = SSBOPointLightData;

    // Contiguous array of point lights (needed to be continuous for SSBO)
    std::vector<LightType> lights;
};

} // namespace vrm
