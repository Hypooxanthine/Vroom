#include "Vroom/Asset/Parsing/SceneParsing.h"

using namespace vrm;

#include "Vroom/Asset/Parsing/Json.h"

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Assert.h"

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"

static bool HandleEntityTransformComponent(SceneData::SceneNode& e, const json& params)
{
  auto tc = std::make_unique<SceneData::TransformComponent>();

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected TransformComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);
    CHECK_ATTR_ARRAY(param, value);

    glm::vec3* outVec;
    if (nameVal == "Position")
      outVec = &tc->position;
    else if (nameVal == "Rotation")
      outVec = &tc->rotation;
    else if (nameVal == "Scale")
      outVec = &tc->scale;
    else
      CHECK(false, "Unsupported TransformComponent parameter {}", nameVal);

    CHECK(GetParamValue(value, *outVec), "Error while parsing vec3");
  }

  e.components[std::type_index(typeid(SceneData::TransformComponent))] = std::move(tc);

  return true;
}

static bool HandleEntityMeshComponent(SceneData::SceneNode& e, const json& params)
{
  auto mc = std::make_unique<SceneData::MeshComponent>();

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected MeshComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);
    CHECK(nameVal == "ResourceName", "Unsupported MeshComponent parameter {}", nameVal);
    CHECK(mc->resourceName.empty(), "Only one ResourceName parameter is supported");
    CHECK_ATTR_STRING(param, value);

    mc->resourceName = valueVal;
  }

  e.components[std::type_index(typeid(SceneData::MeshComponent))] = std::move(mc);

  return true;
}

static bool HandleEntityScriptComponent(SceneData::SceneNode& e, const json& params)
{
  std::string scriptName;

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected ScriptComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);
    CHECK(nameVal == "ScriptName", "Unsupported ScriptComponent parameter {}", nameVal);
    CHECK(scriptName.empty(), "Only one ScriptName parameter is supported");
    CHECK_ATTR_STRING(param, value);

    scriptName = valueVal;
  }
  
  auto sc = std::make_unique<SceneData::ScriptComponent>();
  sc->resourceName = scriptName;

  e.components[std::type_index(typeid(SceneData::ScriptComponent))] = std::move(sc);

  return true;
}

static bool HandleEntityDirectionalLightComponent(SceneData::SceneNode& e, const json& params)
{
  auto dlc = std::make_unique<SceneData::DirectionalLightComponent>();

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected DirectionalLightComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);

    if (nameVal == "Color")
    {
      CHECK_ATTR_ARRAY(param, value);
      CHECK(GetParamValue(value, dlc->color), "Error while parsing color parameter");
    }
    else if (nameVal == "Intensity")
    {
      CHECK_ATTR_FLOAT(param, value);
      CHECK(GetParamValue(value, dlc->intensity), "Error while parsing intensity parameter");
    }
    else
      CHECK(false, "Unsupported DirectionalLightComponent parameter {}", nameVal);
  }

  e.components[std::type_index(typeid(SceneData::PointLightComponent))] = std::move(dlc);

  return true;
}

static bool HandleEntityPointLightComponent(SceneData::SceneNode& e, const json& params)
{
  auto plc = std::make_unique<SceneData::PointLightComponent>();

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected PointLightComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);

    if (nameVal == "Color")
    {
      CHECK_ATTR_ARRAY(param, value);
      CHECK(GetParamValue(value, plc->color), "Error while parsing color parameter");
    }
    else if (nameVal == "Intensity")
    {
      CHECK_ATTR_FLOAT(param, value);
      CHECK(GetParamValue(value, plc->intensity), "Error while parsing intensity parameter");
    }
    else if (nameVal == "Radius")
    {
      CHECK_ATTR_FLOAT(param, value);
      CHECK(GetParamValue(value, plc->radius), "Error while parsing radius parameter");
    }
    else
      CHECK(false, "Unsupported PointLightComponent parameter {}", nameVal);
  }

  e.components[std::type_index(typeid(SceneData::PointLightComponent))] = std::move(plc);

  return true;
}

static bool HandleEntityNodeComponents(SceneData::SceneNode& node, const json& component)
{
  CHECK_ATTR_STRING(component, type);
  CHECK_ATTR_ARRAY(component, params);

  static const std::unordered_map<std::string, std::function<bool(SceneData::SceneNode&, const json&)>> dispatch{
    {"TransformComponent", HandleEntityTransformComponent},
    {"MeshComponent", HandleEntityMeshComponent},
    {"ScriptComponent", HandleEntityScriptComponent},
    {"DirectionalLightComponent", HandleEntityDirectionalLightComponent},
    {"PointLightComponent", HandleEntityPointLightComponent}
  };

  CHECK(dispatch.contains(typeVal), "Component of type {} is not supported", typeVal);
  CHECK(dispatch.at(typeVal)(node, params), "Error while parsing components params {}", params.dump(2));

  return true;
}

static bool HandleSceneNode(SceneData& scene, const json& node)
{
  CHECK_ATTR_STRING(node, type);
  CHECK_ATTR_STRING(node, name);

  CHECK(typeVal == "Root" || typeVal == "Entity", "Scene node of type {} is not supported", typeVal);

  SceneData::SceneNode sceneNode;
  sceneNode.name = nameVal;

  if (typeVal == "Entity")
  {
    sceneNode.type = SceneData::SceneNode::EType::eEntity;
    CHECK_ATTR_STRING(node, parent);

    sceneNode.parent = parentVal;

    if (!node.contains("components"))
    {
      // No component given, we are done
      scene.addNode(std::move(sceneNode));
      return true;
    }

    CHECK_ATTR_ARRAY(node, components);

    for (const auto& jsonComponent : components)
    {
      CHECK(jsonComponent.is_object(), "Unexpected component {}", jsonComponent.dump(2));
      CHECK(HandleEntityNodeComponents(sceneNode, jsonComponent), "Error while parsing component {}", jsonComponent.dump(2));
    }
  }
  else if (typeVal == "Root")
  {
    sceneNode.type = SceneData::SceneNode::EType::eRoot;
  }

  scene.addNode(std::move(sceneNode));
  return true;
}

bool SceneParsing::Parse(const nlohmann::json& jsonData, SceneData& outSceneData)
{
  SceneData out;

  CHECK_ATTR_ARRAY(jsonData, scene_nodes);

  // auto it = std::find_if(sceneNodes.begin(), sceneNodes.end(), [](const json &node) {
  //   return node.contains("type") && node.at("type") == "Root";
  // });
  // CHECK(it != sceneNodes.end(), "Could not find a node of type \"Root\"");
  // const auto& rootNode = *it;

  for (const auto& sceneNode : scene_nodes)
  {
    if (!HandleSceneNode(out, sceneNode))
    {
      VRM_LOG_ERROR("Error while parsing scene node {}", sceneNode.dump(2));
      return false;
    }
  }

  outSceneData = std::move(out);
  return true;
}

void SceneParsing::Serialize(const SceneData& sceneData, nlohmann::json& outJson)
{
}
