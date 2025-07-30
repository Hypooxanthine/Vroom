#include "Vroom/Asset/AssetData/ComponentData.h"

#include <filesystem>
#include "Vroom/Asset/AssetManager.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"

using namespace vrm;

std::string ComponentData::formatPath(const std::string& path)
{
  std::string out = path;
  auto pos = out.find(L'\\');

  while (pos != out.npos)
  {
    out[pos] = '/';
    pos = out.find(L'\\', pos);
  }

  return out;
}

// ------------------------------------------------------
// TransformComponentData

void TransformComponentData::addToEntity(Entity& entity)
{
  auto& tc = entity.getComponent<vrm::TransformComponent>();
  tc.setPosition(position);
  tc.setRotation(rotation);
  tc.setScale(scale);
}

void TransformComponentData::getFromEntity(const Entity& entity)
{
  const auto& tc = entity.getComponent<vrm::TransformComponent>();
  this->position = tc.getPosition();
  this->rotation = tc.getRotation();
  this->scale = tc.getScale();
}

json TransformComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "TransformComponent";
  json& params = j["params"] = json::array();
  
  // Position
  {
    json& posParam = params.emplace_back();
    posParam["name"] = "Position";
    json& pos = posParam["value"];
    to_json(pos, this->position);
  }
  
  // Rotation
  {
    json& rotParam = params.emplace_back();
    rotParam["name"] = "Rotation";
    json& rot = rotParam["value"];
    to_json(rot, this->rotation);
  }

  // Scale
  {
    json& scaleParam = params.emplace_back();
    scaleParam["name"] = "Scale";
    json& scale = scaleParam["value"];
    to_json(scale, this->scale);
  }

  return std::move(j);
}

bool TransformComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "TransformComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "TransformComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected TransformComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "TransformComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "TransformComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "Position")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_array(), "Expected array for Position value");
      this->position = param["value"];
    }
    else if (nameVal == "Rotation")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_array(), "Expected array for Rotation value");
      this->rotation = param["value"];
    }
    else if (nameVal == "Scale")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_array(), "Expected array for Scale value");
      this->scale = param["value"];
    }
    else
    {
      VRM_LOG_ERROR("Unsupported TransformComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// MeshComponentData

void MeshComponentData::addToEntity(Entity& entity)
{
  auto& mc = entity.addComponent<MeshComponent>();
  mc.setMesh(AssetManager::Get().getAsset<MeshAsset>(resourceName));
  mc.setCastsShadow(this->castsShadow);
  mc.setVisible(this->visible);
}

void MeshComponentData::getFromEntity(const Entity& entity)
{
  const auto& mc = entity.getComponent<MeshComponent>();
  this->resourceName = mc.getMesh()->getFilePath();
  this->castsShadow = mc.doesCastShadow();
  this->visible = mc.isVisible();
}

json MeshComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "MeshComponent";
  json& params = j["params"] = json::array();

  // ResourceName
  {
    json& rscParam = params.emplace_back();
    rscParam["name"] = "ResourceName";
    json& rsc = rscParam["value"];
    rsc = formatPath(this->resourceName);
  }

  {
    json& castsShadowParam = params.emplace_back();
    castsShadowParam["name"] = "CastsShadow";
    json& castsShadow = castsShadowParam["value"];
    castsShadow = this->castsShadow;
  }

  {
    json& visibleParam = params.emplace_back();
    visibleParam["name"] = "Visible";
    json& visibleVal = visibleParam["value"];
    visibleVal = this->visible;
  }

  return std::move(j);
}

bool MeshComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "MeshComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "MeshComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected MeshComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "MeshComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "MeshComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "ResourceName")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_string(), "Expected string for ResourceName value");
      this->resourceName = formatPath(param["value"]);
    }
    else if (nameVal == "CastsShadow")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_boolean(), "Expected boolean for CastsShadow value");
      this->castsShadow = param["value"];
    }
    else if (nameVal == "Visible")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_boolean(), "Expected boolean for Visible value");
      this->visible = param["value"];
    }
    else
    {
      VRM_LOG_ERROR("Unsupported MeshComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// SkyboxComponentData

void SkyboxComponentData::addToEntity(Entity& entity)
{
  auto& sc = entity.addComponent<SkyboxComponent>();
  sc.setCubemapAsset(AssetManager::Get().getAsset<CubemapAsset>(resourceName));
}

void SkyboxComponentData::getFromEntity(const Entity& entity)
{
  const auto& mc = entity.getComponent<SkyboxComponent>();
  this->resourceName = mc.getCubemapAsset()->getFilePath();
}

json SkyboxComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "SkyboxComponent";
  json& params = j["params"] = json::array();

  // ResourceName
  {
    json& rscParam = params.emplace_back();
    rscParam["name"] = "ResourceName";
    json& rsc = rscParam["value"];
    rsc = formatPath(this->resourceName);
  }

  return std::move(j);
}

bool SkyboxComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "SkyboxComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "SkyboxComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected SkyboxComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "SkyboxComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "SkyboxComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "ResourceName")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_string(), "Expected string for ResourceName value");
      this->resourceName = formatPath(param["value"]);
    }
    else
    {
      VRM_LOG_ERROR("Unsupported SkyboxComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// DirectionalLightComponentData

void DirectionalLightComponentData::addToEntity(Entity& entity)
{
  auto& dlc = entity.addComponent<DirectionalLightComponent>();
  dlc.color = color;
  dlc.intensity = intensity;
  dlc.castsShadows = castsShadows;
}

void DirectionalLightComponentData::getFromEntity(const Entity& entity)
{
  const auto& dlc = entity.getComponent<DirectionalLightComponent>();
  this->color = dlc.color;
  this->intensity = dlc.intensity;
  this->castsShadows = dlc.castsShadows;
}

json DirectionalLightComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "DirectionalLightComponent";
  json& params = j["params"] = json::array();

  {
    json& colorParam = params.emplace_back();
    colorParam["name"] = "Color";
    json& color = colorParam["value"];
    to_json(color, this->color);
  }

  {
    json& intensityParam = params.emplace_back();
    intensityParam["name"] = "Intensity";
    json& intensity = intensityParam["value"];
    to_json(intensity, this->intensity);
  }

  {
    json& castsShadowsParam = params.emplace_back();
    castsShadowsParam["name"] = "CastsShadows";
    json& castsShadows = castsShadowsParam["value"];
    castsShadows = this->castsShadows;
  }

  return std::move(j);
}

bool DirectionalLightComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "DirectionalLightComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "DirectionalLightComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected DirectionalLightComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "DirectionalLightComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "DirectionalLightComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "Color")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_array(), "Expected array for Color value");
      this->color = param["value"];
    }
    else if (nameVal == "Intensity")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for Intensity value");
      this->intensity = param["value"];
    }
    else if (nameVal == "CastsShadows")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_boolean(), "Expected boolean for CastsShadows value");
      this->castsShadows = param["value"];
    }
    else
    {
      VRM_LOG_ERROR("Unsupported DirectionalLightComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// PointLightComponentData

void PointLightComponentData::addToEntity(Entity& entity)
{
  auto& plc = entity.addComponent<::PointLightComponent>();
  plc.color = color;
  plc.intensity = intensity;
  plc.radius = radius;
  plc.smoothRadius = smoothRadius;
  plc.constantAttenuation = constantAttenuation;
  plc.linearAttenuation = linearAttenuation;
  plc.quadraticAttenuation = quadraticAttenuation;
}

void PointLightComponentData::getFromEntity(const Entity& entity)
{
  const auto& plc = entity.getComponent<::PointLightComponent>();
  this->color = plc.color;
  this->intensity = plc.intensity;
  this->radius = plc.radius;
  this->smoothRadius = plc.smoothRadius;
  this->constantAttenuation = plc.constantAttenuation;
  this->linearAttenuation = plc.linearAttenuation;
  this->quadraticAttenuation = plc.quadraticAttenuation;
}

json PointLightComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "PointLightComponent";
  json& params = j["params"] = json::array();

  {
    json& colorParam = params.emplace_back();
    colorParam["name"] = "Color";
    json& color = colorParam["value"];
    to_json(color, this->color);
  }

  {
    json& intensityParam = params.emplace_back();
    intensityParam["name"] = "Intensity";
    json& intensity = intensityParam["value"];
    to_json(intensity, this->intensity);
  }

  {
    json& radiusParam = params.emplace_back();
    radiusParam["name"] = "Radius";
    json& radius = radiusParam["value"];
    to_json(radius, this->radius);
  }

  {
    json& smoothRadiusParam = params.emplace_back();
    smoothRadiusParam["name"] = "SmoothRadius";
    json& smoothRadius = smoothRadiusParam["value"];
    to_json(smoothRadius, this->smoothRadius);
  }

  {
    json& constantAttenuationParam = params.emplace_back();
    constantAttenuationParam["name"] = "ConstantAttenuation";
    json& constantAttenuation = constantAttenuationParam["value"];
    to_json(constantAttenuation, this->constantAttenuation);
  }

  {
    json& linearAttenuationParam = params.emplace_back();
    linearAttenuationParam["name"] = "LinearAttenuation";
    json& linearAttenuation = linearAttenuationParam["value"];
    to_json(linearAttenuation, this->linearAttenuation);
  }

  {
    json& quadraticAttenuationParam = params.emplace_back();
    quadraticAttenuationParam["name"] = "QuadraticAttenuation";
    json& quadraticAttenuation = quadraticAttenuationParam["value"];
    to_json(quadraticAttenuation, this->quadraticAttenuation);
  }
  return std::move(j);
}

bool PointLightComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "PointLightComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "PointLightComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected PointLightComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "PointLightComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "PointLightComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "Color")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_array(), "Expected array for Color value");
      this->color = param["value"];
    }
    else if (nameVal == "Intensity")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for Intensity value");
      this->intensity = param["value"];
    }
    else if (nameVal == "Radius")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for Radius value");
      this->radius = param["value"];
    }
    else if (nameVal == "SmoothRadius")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for SmoothRadius value");
      this->smoothRadius = param["value"];
    }
    else if (nameVal == "ConstantAttenuation")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for ConstantAttenuation value");
      this->constantAttenuation = param["value"];
    }
    else if (nameVal == "LinearAttenuation")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for LinearAttenuation value");
      this->linearAttenuation = param["value"];
    }
    else if (nameVal == "QuadraticAttenuation")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_number_float(), "Expected float for QuadraticAttenuation value");
      this->quadraticAttenuation = param["value"];
    }
    else
    {
      VRM_LOG_ERROR("Unsupported PointLightComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// ScriptComponentData

void ScriptComponentData::addToEntity(Entity& entity)
{
  VRM_CHECK_MSG(ScriptEngine::Get().isScriptRegistered(resourceName), "Script {} is not registered", resourceName);
  std::unique_ptr<::ScriptComponent> sc;
  sc.reset(ScriptEngine::Get().createScriptComponent(resourceName));
  entity.addScriptComponent(std::move(sc));
}

void ScriptComponentData::getFromEntity(const Entity& entity)
{
  const auto& sc = entity.getScriptComponent();
  this->resourceName = sc.getScriptName();
}

json ScriptComponentData::serialize() const
{
  json j = json::object();
  j["type"] = "ScriptComponent";
  json& params = j["params"] = json::array();

  {
    json& scriptNameParam = params.emplace_back();
    scriptNameParam["name"] = "ScriptName";
    json& scriptName = scriptNameParam["value"];
    scriptName = formatPath(this->resourceName);
  }

  return std::move(j);
}

bool ScriptComponentData::deserialize(const json& j)
{
  VRM_CHECK_RET_FALSE_MSG(j.is_object(), "ScriptComponentData: Expected an object, got {}", j.dump(2));
  VRM_CHECK_RET_FALSE_MSG(j.contains("params"), "ScriptComponentData: Missing 'params' field");

  const auto& params = j["params"];
  for (const auto& param : params)
  {
    VRM_CHECK_RET_FALSE_MSG(param.is_object(), "Unexpected ScriptComponent parameter {}", param.dump(2));
    VRM_CHECK_RET_FALSE_MSG(param.contains("name"), "ScriptComponent parameter missing 'name' attribute");
    VRM_CHECK_RET_FALSE_MSG(param.contains("value"), "ScriptComponent parameter missing 'value' attribute");

    const std::string nameVal = param["name"];
    if (nameVal == "ScriptName")
    {
      VRM_CHECK_RET_FALSE_MSG(param["value"].is_string(), "Expected string for ScriptName value");
      this->resourceName = formatPath(param["value"]);
    }
    else
    {
      VRM_LOG_ERROR("Unsupported ScriptComponent parameter {}", nameVal);
      return false;
    }
  }

  return true;
}

// ------------------------------------------------------
// NLOHMANN SERIAL FUNCTIONS

namespace nlohmann
{
    
  void to_json(json& j, const ComponentData& component)
  {
    j = component.serialize();
  }

  void from_json(const json& j, std::unique_ptr<ComponentData>& component)
  {
    VRM_CHECK_THROW_MSG(j.is_object(), "Expected an object for ComponentData, got {}", j.dump(2));

    const std::string type = j["type"];
    if (type == "TransformComponent")
    {
      auto tc = std::make_unique<TransformComponentData>();
      VRM_CHECK_THROW_MSG(tc->deserialize(j), "Failed to deserialize TransformComponentData");
      component = std::move(tc);
    }
    else if (type == "MeshComponent")
    {
      auto mc = std::make_unique<MeshComponentData>();
      VRM_CHECK_THROW_MSG(mc->deserialize(j), "Failed to deserialize MeshComponentData");
      component = std::move(mc);
    }
    else if (type == "SkyboxComponent")
    {
      auto sc = std::make_unique<SkyboxComponentData>();
      VRM_CHECK_THROW_MSG(sc->deserialize(j), "Failed to deserialize SkyboxComponentData");
      component = std::move(sc);
    }
    else if (type == "DirectionalLightComponent")
    {
      auto dlc = std::make_unique<DirectionalLightComponentData>();
      VRM_CHECK_THROW_MSG(dlc->deserialize(j), "Failed to deserialize DirectionalLightComponentData");
      component = std::move(dlc);
    }
    else if (type == "PointLightComponent")
    {
      auto plc = std::make_unique<PointLightComponentData>();
      VRM_CHECK_THROW_MSG(plc->deserialize(j), "Failed to deserialize PointLightComponentData");
      component = std::move(plc);
    }
    else if (type == "ScriptComponent")
    {
      auto sc = std::make_unique<ScriptComponentData>();
      VRM_CHECK_THROW_MSG(sc->deserialize(j), "Failed to deserialize ScriptComponentData");
      component = std::move(sc);
    }
    else
    {
      VRM_LOG_ERROR("Unsupported component type: {}", type);
    }
  }

}
