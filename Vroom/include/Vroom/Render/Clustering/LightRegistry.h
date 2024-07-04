#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>

#include <glm/glm.hpp>

#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Render/RawShaderData/SSBOPointLightData.h"

namespace vrm
{

/**
 * @brief A registry for point lights. This class is used to keep track of point lights in the scene.
 * In the flow of the program, you should call beginFrame() at the beginning of the frame, submitPointLight() for each point light in the scene, and endFrame() at the end of the frame.
 * The class will keep track of the point lights, and provide the data in a format that can be used in a SSBO.
 * When updating the SSBO, you should first check shouldResetSSBO(), and if it returns true, you should fill the SSBO with the data from getPointLightData().
 * If shouldResetSSBO() returns false, you should check getChangedPointLights() to see which point lights have changed, and update the SSBO with the new data from getPointLights().
 * This way, you can optimize the SSBO updates, and only update the data that has changed when no light has been added or removed.
 * This system supposes that lights won't be added or removed frequently (in this case, we'll do a lot of resets in the SSBO, which is not optimal and could maybe be improved by 
 * allocating more data than needed in the SSBO, just like a std::vector does, and other things).
 * 
 */
class LightRegistry
{
public:
    LightRegistry() = default;
    LightRegistry(const LightRegistry&) = default;
    LightRegistry(LightRegistry&&) = default;
    ~LightRegistry() = default;

    LightRegistry& operator=(const LightRegistry&) = default;
    LightRegistry& operator=(LightRegistry&&) = default;

    void beginFrame();

    void submitPointLight(const PointLightComponent& pointLight, const glm::vec3& position, const std::string& identifier);

    void endFrame();

    /**
     * @brief Get the point light data. The data is relevant for the last ended frame.
     * 
     * @return const SSBOPointLightData* The point light data.
     */
    const SSBOPointLightData* getPointLightData() const;

    /**
     * @brief Get the point lights. The data is relevant for the last ended frame.
     * 
     * @return const std::vector<SSBOPointLightData>& The point lights.
     */
    const std::vector<SSBOPointLightData>& getPointLights() const;

    /**
     * @brief Get the point light count. The count is relevant for the last ended frame.
     * 
     * @return size_t The number of point lights.
     */
    size_t getPointLightCount() const;

    /**
     * @brief Get whether the SSBO should be reset. This is useful for updating the SSBO.
     * The SSBO should be reset when a light has been added or removed. In this case, you should just fill the SSBO with the new data from getPointLightData().
     * 
     * @return bool Whether the SSBO should be reset.
     */
    bool shouldResetSSBO() const;

    /**
     * @brief Get the point lights that have changed in the last frame. This is useful for updating the SSBO.
     * Should be called after endFrame(), and before beginFrame() (beginFrame clears the list of changed point lights).
     * The list stores the indices of the point lights that have changed since the last frame. Thus, you should get the new point lights data from getPointLights(), and update
     * the SSBO with it.
     * 
     * @return const std::list<size_t>& The list of point light indices that have changed.
     */
    const std::list<size_t>& getChangedPointLights() const;

private:
    // Contiguous array of point lights
    std::vector<SSBOPointLightData> m_PointLights, m_NextFramePointLights;

    // A fast way to retrieve a point light by its identifier
    std::unordered_map<std::string, size_t> m_PointLightMap, m_NextFramePointLightMap;

    std::unordered_set<size_t> m_RemovedPointLights;

    // For keeping track of change
    std::list<size_t> m_ChangedPointLights;
    bool m_ShouldResetSSBO = false;
};

} // namespace vrm
