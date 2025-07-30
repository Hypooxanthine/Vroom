#pragma once

#include <typeindex>
#include <glm/glm.hpp>

#include "Vroom/Asset/Parsing/Json.h"

namespace vrm
{

  class Entity;

  struct ComponentData
  {
    virtual ComponentData* clone() const = 0;

    virtual void addToEntity(Entity& entity) = 0;
    virtual void getFromEntity(const Entity& entity) = 0;

    virtual nlohmann::json serialize() const = 0;
    virtual bool deserialize(const nlohmann::json& j) = 0;
  };

  struct TransformComponentData : ComponentData
  {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }

    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct MeshComponentData : ComponentData
  {
    std::string resourceName;
    bool castsShadow;
    bool visible = true; // If not present in serialized data, sets it to visible by default

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }
    
    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct SkyboxComponentData : ComponentData
  {
    std::string resourceName;

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }
    
    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct DirectionalLightComponentData : ComponentData
  {
    glm::vec3 color;
    float intensity;
    bool castsShadows;

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }
    
    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct PointLightComponentData : ComponentData
  {
    glm::vec3 color;
    float intensity;
    float radius;
    float smoothRadius;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }

    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct ScriptComponentData : ComponentData
  {
    std::string resourceName;

    inline ComponentData* clone() const override
    {
      using ThisType = std::decay_t<decltype(*this)>;
      return new ThisType(*this);
    }
    
    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

}