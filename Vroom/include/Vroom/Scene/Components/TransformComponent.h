#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vrm
{

/**
 * @brief Transform component.
 * 
 * A transform component is a simple component that stores the position, rotation and scale of an entity.
 * 
 * @todo Cache the transform matrix.
 */
struct TransformComponent
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getTransform() const
    {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, scale);
        transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, position);

        return transform;
    }
};

} // namespace vrm