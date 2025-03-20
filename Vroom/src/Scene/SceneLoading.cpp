#include "Vroom/Scene/Scene.h"

#include <nlohmann/json.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Assert.h"

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/ScriptComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"

#define CHECK(x, ...) VRM_CHECK_RET_FALSE_MSG(x, __VA_ARGS__)

using namespace vrm;
using json = nlohmann::json;

#define CHECK_ATTR(x, attr)                                           \
  CHECK(x.contains(#attr), #x " must contain a \"" #attr "\" attribute"); \
  const auto &attr = x.at(#attr)

#define TYPE_CHECK_FUNC(type) is_##type

#define CHECK_ATTR_TYPE(x, attr, type) \
  CHECK_ATTR(x, attr);                 \
  CHECK(IsType<type>(attr), "Attribute " #x " from " #x " must be of type " #type)

#define CHECK_ATTR_STRING(x, attr)  \
  CHECK_ATTR_TYPE(x, attr, std::string); \
  auto attr##Val = x.at(#attr).get<std::string>()

#define CHECK_ATTR_ARRAY(x, attr) \
  CHECK_ATTR_TYPE(x, attr, json::array_t)

#define CHECK_ATTR_OBJECT(x, attr) \
  CHECK_ATTR_TYPE(x, attr, json::object_t)

#define CHECK_ATTR_FLOAT(x, attr) \
  CHECK_ATTR_TYPE(x, attr, float)

template <typename T>
inline consteval std::string_view ToString() { return "Unsupported"; }

template<typename T>
inline bool IsType(const json& value) { return false; }

template<>
inline consteval std::string_view ToString<float>() { return "float"; }

template <>
inline bool IsType<float>(const json& value) { return value.is_number_float(); }

template<>
inline consteval std::string_view ToString<int>() { return "int"; }

template <>
inline bool IsType<int>(const json& value) { return value.is_number_integer(); }

template<>
inline consteval std::string_view ToString<unsigned int>() { return "unsigned int"; }

template <>
inline bool IsType<unsigned int>(const json& value) { return value.is_number_unsigned(); }

template<>
inline consteval std::string_view ToString<std::string>() { return "string"; }

template <>
inline bool IsType<std::string>(const json& value) { return value.is_string(); }

template <>
inline bool IsType<json::array_t>(const json& value) { return value.is_array(); }

template <>
inline bool IsType<json::object_t>(const json& value) { return value.is_object(); }

template <typename T>
static bool GetParamValue(const json& paramValue, T& outValue)
{
  CHECK(IsType<T>(paramValue), "Unexpected type {}", paramValue.type_name());
  outValue = paramValue.get<T>();
  return true;
}

template <glm::length_t L, typename T>
static bool GetParamValue(const json& paramValue, glm::vec<L, T>& outValue)
{
  glm::length_t i = 0;
  for (const auto& value_i : paramValue)
  {
    CHECK(
      i < L,
      "Expected an array of {} elements", L
    );
    CHECK(
      GetParamValue<T>(value_i, outValue[i]),
      "Expected type {}", ToString<T>()
    );
    ++i;
  }

  return true;
}

static bool HandleEntityTransformComponent(Entity& e, const json& params)
{
  glm::vec3 pos = {0.f, 0.f, 0.f};
  glm::vec3 rot = {0.f, 0.f, 0.f};
  glm::vec3 scale = {1.f, 1.f, 1.f};

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected TransformComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);
    CHECK_ATTR_ARRAY(param, value);
    
    glm::vec3* outVec;
    if (nameVal == "Position")
      outVec = &pos;
    else if (nameVal == "Rotation")
      outVec = &rot;
    else if (nameVal == "Scale")
      outVec = &scale;
    else
      CHECK(false, "Unsupported TransformComponent parameter {}", nameVal);

    CHECK(GetParamValue(value, *outVec), "Error while parsing vec3");
  }

  auto& t = e.getComponent<TransformComponent>();
    t.setPosition(pos);
    t.setRotation(rot);
    t.setScale(scale);

  return true;
}

static bool HandleEntityMeshComponent(Entity& e, const json& params)
{
  std::string meshName;

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected MeshComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);
    CHECK(nameVal == "ResourceName", "Unsupported MeshComponent parameter {}", nameVal);
    CHECK(meshName.empty(), "Only one ResourceName parameter is supported");
    CHECK_ATTR_STRING(param, value);

    meshName = valueVal;
  }

  auto& manager = AssetManager::Get();

  auto meshAssetHandle = manager.getAsset<MeshAsset>(meshName);
  e.addComponent<MeshComponent>(meshAssetHandle);

  return true;
}

static bool HandleEntityScriptComponent(Entity& e, const json& params)
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
  
  CHECK(ScriptEngine::Get().isScriptRegistered(scriptName), "Script {} is not registered", scriptName);
  
  auto* rawScriptPtr = ScriptEngine::Get().getScriptFactory(scriptName)();
  auto script = std::unique_ptr<ScriptComponent>(rawScriptPtr);
  e.addScriptComponent(std::move(script));
  
  return true;
}

static bool HandleEntityPointLightComponent(Entity& e, const json& params)
{
  auto& plc = e.addComponent<vrm::PointLightComponent>();

  for (const auto& param : params)
  {
    CHECK(param.is_object(), "Unexpected PointLightComponent parameter {}", param.dump(2));
    CHECK_ATTR_STRING(param, name);

    if (nameVal == "Color")
    {
      CHECK_ATTR_ARRAY(param, value);
      CHECK(GetParamValue(value, plc.color), "Error while parsing color parameter");
    }
    else if (nameVal == "Intensity")
    {
      CHECK_ATTR_FLOAT(param, value);
      CHECK(GetParamValue(value, plc.intensity), "Error while parsing intensity parameter");
    }
    else if (nameVal == "Radius")
    {
      CHECK_ATTR_FLOAT(param, value);
      CHECK(GetParamValue(value, plc.radius), "Error while parsing radius parameter");
    }
    else
      CHECK(false, "Unsupported PointLightComponent parameter {}", nameVal);
  }

  return true;
}

static bool HandleEntityComponents(Entity &e, const json &component)
{
  CHECK_ATTR_STRING(component, type);
  CHECK_ATTR_ARRAY(component, params);

  static const std::unordered_map<std::string, std::function<bool(Entity&, const json&)>> dispatch {
    {"TransformComponent", HandleEntityTransformComponent},
    {"MeshComponent", HandleEntityMeshComponent},
    {"ScriptComponent", HandleEntityScriptComponent},
    {"PointLightComponent", HandleEntityPointLightComponent}
  };

  CHECK(dispatch.contains(typeVal), "Component of type {} is not supported", typeVal);
  CHECK(dispatch.at(typeVal)(e, params), "Error while parsing components params {}", params.dump(2));

  return true;
}

static bool HandleSceneNode(Scene &scene, const json &node)
{
  CHECK_ATTR_STRING(node, type);
  CHECK_ATTR_STRING(node, name);

  if (typeVal == "Root")
  {
    // Scene Root is already added by the Scene on its creation
    return true;
  }
  else if (typeVal == "Entity")
  {
    // Some specific behaviour...
  }
  else
    CHECK(false, "Scene node of type {} is not supported", typeVal);

  // node is designating an entity

  auto e = scene.createEntity(nameVal);

  if (!node.contains("components"))
    // No component given, we are done
    return true;

  CHECK_ATTR_ARRAY(node, components);

  for (const auto &jsonComponent : components)
  {
    CHECK(jsonComponent.is_object(), "Unexpected component {}", jsonComponent.dump(2));
    CHECK(HandleEntityComponents(e, jsonComponent), "Error while parsing component {}", jsonComponent.dump(2));
  }

  return true;
}

bool Scene::loadFromAsset(const SceneInstance &data)
{
  destroyAllEntities();
  const auto &jsonRoot = data.getStaticAsset()->getSceneData();

  CHECK_ATTR_ARRAY(jsonRoot, scene_nodes);

  // auto it = std::find_if(sceneNodes.begin(), sceneNodes.end(), [](const json &node) {
  //   return node.contains("type") && node.at("type") == "Root";
  // });
  // CHECK(it != sceneNodes.end(), "Could not find a node of type \"Root\"");
  // const auto& rootNode = *it;

  for (const auto &sceneNode : scene_nodes)
  {
    if (!HandleSceneNode(*this, sceneNode))
    {
      destroyAllEntities();
      VRM_LOG_ERROR("Error while parsing scene node {}", sceneNode.dump(2));
      return false;
    }
  }

  VRM_LOG_INFO("Scene loaded successfully");

  return true;
}
