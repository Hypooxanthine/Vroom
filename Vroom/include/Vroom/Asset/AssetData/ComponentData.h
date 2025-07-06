#pragma once

#include <typeindex>
#include <glm/glm.hpp>

#include "Vroom/Asset/Parsing/Json.h"

namespace vrm
{

  class Entity;

  struct ComponentData
  {
    virtual ~ComponentData() {}
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

    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct MeshComponentData : ComponentData
  {
    std::string resourceName;
    bool castsShadow;
    
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

    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

  struct ScriptComponentData : ComponentData
  {
    std::string resourceName;
    
    void addToEntity(Entity& entity) override;
    void getFromEntity(const Entity& entity) override;
    nlohmann::json serialize() const override;
    bool deserialize(const nlohmann::json& j) override;
  };

}