#include "Vroom/Scene/Scene.h"

#include <nlohmann/json.hpp>

#include "Vroom/Asset/AssetManager.h"
#include "Vroom/Asset/StaticAsset/SceneAsset.h"
#include "Vroom/Asset/StaticAsset/MeshAsset.h"
#include "Vroom/Core/Assert.h"

#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"

#define CHECK(x, ...) VRM_CHECK_RET_FALSE_MSG(x, __VA_ARGS__)

using namespace vrm;
using json = nlohmann::json;

#define CHECK_ATTR(x, attr)                                           \
  CHECK(x.contains(#attr), #x " must contain a \"" #attr "\" attribute"); \
  const auto &attr = x.at(#attr)

#define TYPE_CHECK_FUNC(type) is_##type

#define CHECK_ATTR_TYPE(x, attr, type) \
  CHECK_ATTR(x, attr);                 \
  CHECK(attr.TYPE_CHECK_FUNC(type)(), "Attribute " #x " from " #x " must be of type " #type)

#define CHECK_ATTR_STRING(x, attr)  \
  CHECK_ATTR_TYPE(x, attr, string); \
  auto attr##Val = x.get<std::string>()

#define CHECK_ATTR_ARRAY(x, attr) \
  CHECK_ATTR_TYPE(x, attr, array)

#define CHECK_ATTR_OBJECT(x, attr) \
  CHECK_ATTR_TYPE(x, attr, object)

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
    
    int i = 0;
    for (const auto& value_i : value)
    {
      CHECK(
        i < 3 && value_i.is_number_float(),
        "TransformComponent {} parameter must contain an array of 3 floats", nameVal
      );
      (*outVec)[i] = value_i.get<float>();
      ++i;
    }
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

  return true;
}

static bool HandleEntityComponents(Entity &e, const json &component)
{
  CHECK_ATTR_STRING(component, type);
  CHECK_ATTR_ARRAY(component, params);

  static const std::unordered_map<std::string, std::function<bool(Entity&, const json&)>> dispatch {
    {"TransformComponent", HandleEntityTransformComponent},
    {"MeshComponent", HandleEntityMeshComponent},
    {"ScriptComponent", HandleEntityScriptComponent}
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
    // Some specific behaviour...
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
    CHECK(HandleSceneNode(*this, sceneNode), "Error while parsing scene node {}", sceneNode.dump(2));
  }

  return true;
}
