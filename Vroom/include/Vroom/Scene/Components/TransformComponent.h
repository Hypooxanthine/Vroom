#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace vrm
{

/**
 * @brief Transform component.
 *
 * A transform component is a simple component that stores the position, rotation and scale of an entity.
 */
  class TransformComponent
  {
  public:
    TransformComponent() = default;

    void setPosition(const glm::vec3& pos)
    {
      position = pos;
      m_Dirty = true;
    }

    void setRotation(const glm::vec3& rot)
    {
      rotation = glm::mod(rot, glm::vec3(glm::two_pi<float>()));
      m_Dirty = true;
    }

    void setScale(const glm::vec3& s)
    {
      scale = s;
      m_Dirty = true;
    }

    const glm::vec3& getPosition() const { return position; }

    const glm::vec3& getRotation() const { return rotation; }

    const glm::vec3& getScale() const { return scale; }

    const glm::mat4& getTransform() const
    {
      if (m_Dirty)
      {
        computeTransform();
        m_Dirty = false;
      }

      return m_Transform;
    }

  private:
    void computeTransform() const
    {
      m_Transform = glm::mat4(1.0f);
      m_Transform = glm::translate(m_Transform, position);
      m_Transform = glm::rotate(m_Transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      m_Transform = glm::rotate(m_Transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      m_Transform = glm::rotate(m_Transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      m_Transform = glm::scale(m_Transform, scale);
    }


  private:
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    mutable glm::mat4 m_Transform = glm::mat4(1.0f);
    mutable bool m_Dirty = true;
  };

} // namespace vrm