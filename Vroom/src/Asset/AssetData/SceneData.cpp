#include "Vroom/Asset/AssetData/SceneData.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
#include "Vroom/Scene/Components/ScriptComponent.h"

using namespace vrm;

static json GetBasicParameterJson(const std::string& paramName)
{
  json j = json::object();
  j["name"] = paramName;
  j["value"];

  return std::move(j);
}

static json GetBasicComponentJson(const std::string& type, bool hasParams = true)
{
  json j = json::object();
  j["type"] = type;

  if (hasParams)
  {
    j["params"] = json::array();
  }

  return std::move(j);
}

bool SceneData::TransformComponent::addToEntity(Entity& entity)
{
  auto& tc = entity.getComponent<::TransformComponent>();
  tc.setPosition(position);
  tc.setRotation(rotation);
  tc.setScale(scale);

  return true;
}

json SceneData::TransformComponent::serialize() const
{
  json j = GetBasicComponentJson("TransformComponent");
  auto& j_params = j["params"];
  
  // Position
  {
    json& pos = j_params.emplace_back(GetBasicParameterJson("Position")).at("value");
    to_json(pos, this->position);
  }
  
  // Rotation
  {
    json& rot = j_params.emplace_back(GetBasicParameterJson("Rotation")).at("value");
    to_json(rot, this->rotation);
  }
  
  // Scale
  {
    json& sc = j_params.emplace_back(GetBasicParameterJson("Scale")).at("value");
    to_json(sc, this->scale);
  }

  return std::move(j);
}

bool SceneData::MeshComponent::addToEntity(Entity& entity)
{
  auto& mc = entity.addComponent<::MeshComponent>();
  mc.setMesh(AssetManager::Get().getAsset<MeshAsset>(resourceName));
  mc.setCastsShadow(this->castsShadow);

  return true;
}

json SceneData::MeshComponent::serialize() const
{
  json j = GetBasicComponentJson("MeshComponent");
  json& j_params = j["params"];

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("ResourceName")).at("value");
    to_json(j, this->resourceName);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("CastsShadow")).at("value");
    to_json(j, this->castsShadow);
  }

  return std::move(j);
}

bool SceneData::DirectionalLightComponent::addToEntity(Entity& entity)
{
  auto& dlc = entity.addComponent<::DirectionalLightComponent>();
  dlc.color = color;
  dlc.intensity = intensity;
  dlc.castsShadows = castsShadows;

  return true;
}

json SceneData::DirectionalLightComponent::serialize() const
{
  json j = GetBasicComponentJson("DirectionalLightComponent");
  json& j_params = j["params"];

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("Color")).at("value");
    to_json(j, this->color);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("Intensity")).at("value");
    to_json(j, this->intensity);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("CastsShadows")).at("value");
    to_json(j, this->castsShadows);
  }

  return std::move(j);
}

bool SceneData::PointLightComponent::addToEntity(Entity& entity)
{
  auto& plc = entity.addComponent<::PointLightComponent>();
  plc.color = color;
  plc.intensity = intensity;
  plc.radius = radius;
  plc.constantAttenuation = constantAttenuation;
  plc.linearAttenuation = linearAttenuation;
  plc.quadraticAttenuation = quadraticAttenuation;

  return true;
}

json SceneData::PointLightComponent::serialize() const
{
  json j = GetBasicComponentJson("PointLightComponent");
  json& j_params = j["params"];

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("Color")).at("value");
    to_json(j, this->color);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("Intensity")).at("value");
    to_json(j, this->intensity);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("Radius")).at("value");
    to_json(j, this->radius);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("ConstantAttenuation")).at("value");
    to_json(j, this->constantAttenuation);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("LinearAttenuation")).at("value");
    to_json(j, this->linearAttenuation);
  }

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("QuadraticAttenuation")).at("value");
    to_json(j, this->quadraticAttenuation);
  }

  return std::move(j);
}

bool SceneData::ScriptComponent::addToEntity(Entity& entity)
{
  VRM_CHECK_RET_FALSE_MSG(ScriptEngine::Get().isScriptRegistered(resourceName), "Script {} is not registered", resourceName);
  std::unique_ptr<::ScriptComponent> sc;
  sc.reset(ScriptEngine::Get().createScriptComponent(resourceName));
  entity.addScriptComponent(std::move(sc));

  return true;
}

json SceneData::ScriptComponent::serialize() const
{
  json j = GetBasicComponentJson("ScriptComponent");
  json& j_params = j["params"];

  {
    json& j = j_params.emplace_back(GetBasicParameterJson("ScriptName")).at("value");
    to_json(j, this->resourceName);
  }

  return std::move(j);
}

namespace nlohmann
{

  void to_json(json& j, const SceneData::Component* component)
  {
    if (component != nullptr)
      j = component->serialize();
  }

  void to_json(json& j, const SceneData::SceneNode& node)
  {
    j = json::object();
    j["name"] = node.name;
    j["type"] = node.type;

    auto& j_parents = (j["parent"] = nullptr);

    if (!node.parent.empty())
    {
      j_parents = node.parent;
    }

    if (node.components.empty())
    {
      return; 
    }

    auto& j_components = (j["components"] = json::array());

    for (const auto& [_, component] : node.components)
    {
      json& j = j_components.emplace_back();
      j = component.get();
    }
  }

  void to_json(json& j, const SceneData& scene)
  {
    j = json::object();
    json& nodes = j["scene_nodes"] = json::array();

    for (const SceneData::SceneNode& node : scene.getNodes())
    {
      nodes.emplace_back() = node;
    }
  }

}
