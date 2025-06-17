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
    TransformComponent()
    {
      _setFrameDirty();
    }

    TransformComponent(const TransformComponent&) = default;
    TransformComponent& operator=(const TransformComponent&) = default;

    TransformComponent(TransformComponent&&) = default;
    TransformComponent& operator=(TransformComponent&&) = default;

    inline void setPosition(const glm::vec3& pos)
    {
      m_position = pos;
      _setTransformDirty();
      _setFrameDirty();
    }

    inline void setRotation(const glm::vec3& rot)
    {
      m_rotation = glm::mod(rot, glm::vec3(glm::two_pi<float>()));
      _setTransformDirty();
      _setFrameDirty();
    }

    inline void setScale(const glm::vec3& s)
    {
      m_scale = s;
      _setTransformDirty();
      _setFrameDirty();
    }

    inline const glm::vec3& getPosition() const { return m_position; }

    inline const glm::vec3& getRotation() const { return m_rotation; }

    inline const glm::vec3& getScale() const { return m_scale; }

    inline const glm::mat4& getTransform() const
    {
      _computeTransformCheck();

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
      friend class Scene;

    private:
      
      inline static void setFrameDirty(TransformComponent& tc, bool dirty)
      {
        tc._setFrameDirty(dirty);
      }

      // Assumes parent's globals are clear
      inline static void computeGlobals(TransformComponent& tc, const TransformComponent& parent)
      {
        tc.m_globalTransform = parent.getGlobalTransform() * tc.getTransform();
        tc.m_globalPosition = tc.m_globalTransform[3];

        tc.m_globalScale = glm::vec3
        {
          glm::length(glm::vec3(tc.m_globalTransform[0])),
          glm::length(glm::vec3(tc.m_globalTransform[1])),
          glm::length(glm::vec3(tc.m_globalTransform[2]))
        };

        glm::mat3 rotation;
        rotation[0] = glm::vec3(tc.m_globalTransform[0]) / tc.m_globalScale.x;
        rotation[1] = glm::vec3(tc.m_globalTransform[1]) / tc.m_globalScale.y;
        rotation[2] = glm::vec3(tc.m_globalTransform[2]) / tc.m_globalScale.z;

        tc.m_globalRotation = glm::quat_cast(rotation);
      }
    };

  private:

    inline void _setTransformDirty(bool dirty = true) const
    {
      m_dirtyFlags.set(EDirtyFlags::eTransform, dirty);
    }

    inline void _setFrameDirty(bool dirty = true) const
    {
      m_dirtyFlags.set(EDirtyFlags::eFrame, dirty);
    }

    inline void _computeTransformCheck() const
    {
      if (isTransformDirty())
      {
        _computeTransformUncheck();
      }
    }

    inline void _computeTransformUncheck() const
    {
      m_transform = glm::mat4(1.0f);
      m_transform = glm::translate(m_transform, m_position);
      m_transform = glm::rotate(m_transform, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      m_transform = glm::rotate(m_transform, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      m_transform = glm::rotate(m_transform, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      m_transform = glm::scale(m_transform, m_scale);
      _setTransformDirty(false);
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