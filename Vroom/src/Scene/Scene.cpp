#include "Vroom/Scene/Scene.h"

#include "Vroom/Core/Application.h"
#include "Vroom/Core/GameLayer.h"

#include "Vroom/Asset/Asset.h"

#include "Vroom/Render/Renderer.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/HierarchyComponent.h"

vrm::FirstPersonCamera vrm::Scene::s_DefaultCamera = {0.1f, 100.f, glm::radians(90.f), 0.f, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f)};

using namespace vrm;

Scene::Scene()
{
  m_Root = createRoot();
  getRoot().getComponentInternal<HierarchyComponent>().parent = Entity();

  // Setting a default camera
  setCamera(&s_DefaultCamera);
}

Scene::~Scene()
{
}

void Scene::init()
{
  Application::Get().getGameLayer()
    .getCustomEvent("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE")
    .bindCallback(
      [this](const vrm::Event &e)
      {
        getCamera().setViewportSize(static_cast<float>(e.newWidth), static_cast<float>(e.newHeight));
      });

  onInit();

  const auto &viewportSize = Renderer::Get().getViewportSize();
  getCamera().setViewportSize(static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y));
}

void Scene::update(float dt)
{
  onUpdate(dt);

  auto viewScripts = m_Registry.view<ScriptHandler>();
  for (auto entity : viewScripts)
  {
    auto &scriptHandler = viewScripts.get<ScriptHandler>(entity);
    scriptHandler.getScript().onUpdate(dt);
  }
}

void Scene::render()
{
  Application &app = Application::Get();
  Renderer &renderer = Renderer::Get();
  renderer.beginScene(getCamera());

  auto viewPointLights = m_Registry.view<PointLightComponent, TransformComponent, NameComponent>();
  for (auto entity : viewPointLights)
  {
    const auto &pointLightComponent = viewPointLights.get<PointLightComponent>(entity);
    const auto &transformComponent = viewPointLights.get<TransformComponent>(entity);
    const auto &nameComponent = viewPointLights.get<NameComponent>(entity);

    renderer.submitPointLight(transformComponent.getPosition(), pointLightComponent, nameComponent.name);
  }

  auto viewMeshes = m_Registry.view<MeshComponent, TransformComponent>();
  for (auto entity : viewMeshes)
  {
    const auto &meshComponent = viewMeshes.get<MeshComponent>(entity);
    if (meshComponent.isVisible() == false)
      continue;
    const auto &transformComponent = viewMeshes.get<TransformComponent>(entity);

    renderer.submitMesh(meshComponent.getMesh(), transformComponent.getTransform());
  }

  onRender();

  renderer.endScene(app.getGameLayer().getFrameBuffer());
}

void Scene::end()
{
  onEnd();

  m_Registry.clear(); // So that entities are destroyed properly
}

Entity Scene::createEntity(const std::string &nameTag)
{
  auto e = createRawEntity(nameTag);
  auto &hierarchy = e.getComponentInternal<HierarchyComponent>();
  hierarchy.parent = m_Root;
  getRoot().getComponentInternal<HierarchyComponent>().children.emplace_back(e.clone());

  return e;
}

Entity Scene::createEntity()
{
  return createEntity("Entity_" + std::to_string(m_EntityCounter++));
}

void Scene::renameEntity(Entity& e, const std::string& name)
{
  VRM_ASSERT_MSG(e != m_Root, "You cannot rename root entity.");
  VRM_ASSERT_MSG(!name.empty(), "You cannot use an empty name.");
  VRM_ASSERT_MSG(!m_EntitiesByName.contains(name), "Another entity is already named \"{}\". Consider using entityExists(const std::string& name) before renaming an entity.", name);
  VRM_ASSERT_MSG(e.isValid(), "Entity is not valid.");
  m_EntitiesByName.erase(e.getName());
  m_EntitiesByName[name] = e.clone();
  e.getComponentInternal<NameComponent>().name = name;

  // @todo Keep entities sorted by name in HierarchyComponent
}

bool Scene::entityExists(const std::string &name)
{
  return m_EntitiesByName.contains(name);
}

Entity Scene::getEntity(entt::entity handle)
{
  VRM_ASSERT_MSG(m_Registry.valid(handle), "Entity is not valid.");

  return Entity(handle, &m_Registry, this);
}

Entity Scene::getEntity(const std::string &name)
{
  VRM_ASSERT_MSG(entityExists(name), "Entity with name " + name + " does not exist.");

  return m_EntitiesByName.at(name);
}

bool Scene::checkEntitiesRelation(const Entity& parent, const Entity& child) const
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  return child.getComponent<HierarchyComponent>().parent == parent;
}

void Scene::setEntitiesRelation(Entity& parent, Entity& child)
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  VRM_ASSERT_MSG(child != m_Root, "You cannot set Root node's parent");

  if (checkEntitiesRelation(parent, child))
    return;

  auto &hParent = parent.getComponentInternal<HierarchyComponent>();
  auto &hChild = child.getComponentInternal<HierarchyComponent>();
  auto &hExParent = hChild.parent.getComponentInternal<HierarchyComponent>();

  hExParent.children.remove(child);
  hChild.parent = parent;
  hParent.children.emplace_back(child.clone());
}

void Scene::destroyEntity(Entity& entity)
{
  VRM_ASSERT_MSG(m_Registry.valid(entity), "Entity is not valid.");
  VRM_ASSERT_MSG(entity != m_Root, "You cannot delete root entity!");

  auto parent = entity.getComponentInternal<HierarchyComponent>().parent;

  destroyEntityRecursive(entity);

  // Removing entity from its parent children
  parent.getComponentInternal<HierarchyComponent>().children.remove(entity);
}

void Scene::destroyEntityRecursive(Entity entity)
{
  auto &h = entity.getComponentInternal<HierarchyComponent>();
  auto children = std::move(h.children);

  for (auto child : children)
  {
    destroyEntityRecursive(child);
  }

  if (entity.hasComponent<ScriptHandler>())
  {
    entity.getComponent<ScriptHandler>().getScript().onDestroy();
  }

  m_Registry.destroy(entity);
  m_EntitiesByName.erase(entity.getComponentInternal<NameComponent>().name);
}

void Scene::destroyAllEntities()
{
  m_Registry.clear();
  m_EntitiesByName.clear();
  m_Root = createRoot();
}

Entity Scene::createRawEntity(const std::string &nameTag)
{
  VRM_ASSERT_MSG(!entityExists(nameTag), "Entity with name " + nameTag + " already exists.");
  auto e = getEntity(m_Registry.create());
  e.addComponent<NameComponent>(nameTag);
  e.addComponent<TransformComponent>();
  auto &hierarchy = e.addComponent<HierarchyComponent>();
  m_EntitiesByName[nameTag] = e;

  return e;
}

Entity Scene::createRoot()
{
  auto e = createRawEntity("Root");
  e.getComponentInternal<HierarchyComponent>().parent = Entity();

  return e;
}
