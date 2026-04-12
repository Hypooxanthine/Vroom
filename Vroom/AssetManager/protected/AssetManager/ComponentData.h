#pragma once

#include <glm/glm.hpp>

#include "AssetManager/Api.h"
#include "nlohmann/json_fwd.hpp"

namespace vrm
{

struct VRM_ASSETMANAGER_API ComponentData
{
  virtual ComponentData* clone() const = 0;

  virtual nlohmann::json serialize() const                    = 0;
  virtual bool           deserialize(const nlohmann::json& j) = 0;

  static std::string formatPath(const std::string& path);
};

struct VRM_ASSETMANAGER_API TransformComponentData : ComponentData
{
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

struct VRM_ASSETMANAGER_API MeshComponentData : ComponentData
{
  std::string resourceName;
  bool        castsShadow;
  bool        visible = true; // If not present in serialized data, sets it to visible by default

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

struct VRM_ASSETMANAGER_API SkyboxComponentData : ComponentData
{
  std::string resourceName;

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

struct VRM_ASSETMANAGER_API DirectionalLightComponentData : ComponentData
{
  glm::vec3 color;
  float     intensity;
  bool      castsShadows;

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

struct VRM_ASSETMANAGER_API PointLightComponentData : ComponentData
{
  glm::vec3 color;
  float     intensity;
  float     radius;
  float     smoothRadius;
  float     constantAttenuation;
  float     linearAttenuation;
  float     quadraticAttenuation;

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

struct VRM_ASSETMANAGER_API ScriptComponentData : ComponentData
{
  std::string resourceName;

  inline ComponentData* clone() const override
  {
    using ThisType = std::decay_t<decltype(*this)>;
    return new ThisType(*this);
  }

  nlohmann::json serialize() const override;
  bool           deserialize(const nlohmann::json& j) override;
};

} // namespace vrm
