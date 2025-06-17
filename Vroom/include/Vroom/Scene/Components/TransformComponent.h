#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <bitset>
#include "Vroom/Api.h"

namespace vrm
{

  /**
   * @brief Transform component.
   *
   * A transform component is a simple component that stores the position, rotation and scale of an entity.
   */
  class VRM_API TransformComponent
  {
  public:
    TransformComponent() = default;

    TransformComponent(const TransformComponent&) = default;
    TransformComponent& operator=(const TransformComponent&) = default;

    TransformComponent(TransformComponent&&) = default;
    TransformComponent& operator=(TransformComponent&&) = default;

    inline void setPosition(const glm::vec3& pos)
    {
      m_position = pos;
      setTransformDirty();
      setFrameDirty();
    }

    inline void setRotation(const glm::vec3& rot)
    {
      m_rotation = glm::mod(rot, glm::vec3(glm::two_pi<float>()));
      setTransformDirty();
      setFrameDirty();
    }

    inline void setScale(const glm::vec3& s)
    {
      m_scale = s;
      setTransformDirty();
      setFrameDirty();
    }

    inline const glm::vec3& getPosition() const { return m_position; }

    inline const glm::vec3& getRotation() const { return m_rotation; }

    inline const glm::vec3& getScale() const { return m_scale; }

    inline const glm::mat4& getTransform() const
    {
      computeTransformCheck();

      return m_transform;
    }

    inline const glm::vec3& getGlobalPosition() const
    {
      return m_globalPosition;
    }

    inline glm::vec3 getGlobalRotation() const
    {
      return glm::eulerAngles(m_globalRotation);
    }

    inline const glm::quat& getGlobalRotationQuat() const
    {
      return m_globalRotation;
    }

    inline const glm::vec3& getGlobalScale() const
    {
      return m_globalScale;
    }

    inline const glm::mat4& getGlobalTransform() const
    {
      return m_globalTransform;
    }

    inline bool isTransformDirty() const
    {
      return m_dirtyFlags.test(EDirtyFlags::eTransform);
    }

    inline bool isFrameDirty() const
    {
      return m_dirtyFlags.test(EDirtyFlags::eFrame);
    }

  public:

    friend struct SceneAttorney;

    // Scene needs to reset the changedThisFrame flag
    struct SceneAttorney
    {
      friend class scene;

    private:
      
      inline static void resetFrameDirty(TransformComponent& tc)
      {
        tc.setFrameDirty(false);
      }

      // Assumes parent's globals are clear
      inline static void computeGlobals(TransformComponent& tc, const TransformComponent& parent)
      {
        tc.m_globalTransform = tc.getTransform() * parent.getGlobalTransform();
        tc.m_globalPosition = tc.getPosition() + parent.getGlobalPosition();
        tc.m_globalScale = tc.getScale() * parent.getGlobalScale();
        tc.m_globalRotation = glm::quat(tc.getRotation()) * parent.getGlobalRotationQuat();
      }
    };

  private:

    inline void setTransformDirty(bool dirty = true) const
    {
      m_dirtyFlags.set(EDirtyFlags::eTransform, dirty);
    }

    inline void setFrameDirty(bool dirty = true) const
    {
      m_dirtyFlags.set(EDirtyFlags::eFrame, dirty);
    }

    inline void computeTransformCheck() const
    {
      if (isTransformDirty())
      {
        computeTransformUncheck();
      }
    }

    inline void computeTransformUncheck() const
    {
      m_transform = glm::mat4(1.0f);
      m_transform = glm::translate(m_transform, m_position);
      m_transform = glm::rotate(m_transform, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      m_transform = glm::rotate(m_transform, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      m_transform = glm::rotate(m_transform, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      m_transform = glm::scale(m_transform, m_scale);
      setTransformDirty(false);

      constexpr auto size = sizeof(m_dirtyFlags);
    }


  private:
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    mutable glm::mat4 m_transform = glm::mat4(1.0f);

    glm::vec3 m_globalPosition = glm::vec3(0.0f);
    glm::quat m_globalRotation = glm::quat(glm::vec3(0.0f));
    glm::vec3 m_globalScale = glm::vec3(1.0f);
    glm::mat4 m_globalTransform = glm::mat4(1.0f);
    
    struct EDirtyFlags
    {
      enum : size_t
      {
        eTransform = 0,
        eFrame,
        eCount
      };
    };

    mutable std::bitset<EDirtyFlags::eCount> m_dirtyFlags;
  };

} // namespace vrm