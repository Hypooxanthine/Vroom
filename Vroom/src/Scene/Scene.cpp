#include "Vroom/Scene/Scene.h"

#include <glm/gtx/euler_angles.hpp>

#include "Vroom/Core/Application.h"
#include "Vroom/Core/GameLayer.h"
#include "Vroom/Core/Profiling.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Components/DirectionalLightComponent.h"
#include "Vroom/Scene/Components/HierarchyComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/PointLightComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Entity.h"

using namespace vrm;

Scene::Scene()
{
  m_renderer.reset(new Renderer());

  m_Root                                                      = createRoot();
  getRoot().getComponentInternal<HierarchyComponent>().parent = Entity();

  // Setting a default camera
  setCamera(&m_defaultCamera);
}

Scene::~Scene() {}

void Scene::init() { onInit(); }

void Scene::update(const DeltaTime& dt)
{
  VRM_PROFILE_SCOPE("Scene::update");
  onUpdate(dt);

  auto viewParticles = m_Registry.view<ParticleSystemComponent>();
  for (auto entity : viewParticles)
  {
    auto& particleSystem = viewParticles.get<ParticleSystemComponent>(entity);
    particleSystem.update(dt);
  }

  auto viewScripts = m_Registry.view<ScriptHandler>();
  for (auto entity : viewScripts)
  {
    auto& scriptHandler = viewScripts.get<ScriptHandler>(entity);
    scriptHandler.getScript().onUpdate(dt);
  }
}

void Scene::render()
{
  VRM_PROFILE_SCOPE("Scene::render");
  _updateGlobalTransforms();

  Renderer& renderer = *m_renderer;
  renderer.beginScene(&m_renderLayout);

  auto viewDirLights =
    m_Registry.view<DirectionalLightComponent, TransformComponent>();
  for (auto&& [e, dl, t] : viewDirLights.each())
  {
    const glm::quat& rot     = t.getGlobalRotationQuat();
    glm::vec4        forward = { 1.f, 0.f, 0.f, 0.f };

    renderer.submitDirectionalLight(static_cast<entt::id_type>(e), dl,
                                    glm::vec3(rot * forward));
  }

  auto viewPointLights =
    m_Registry.view<PointLightComponent, TransformComponent>();
  for (auto&& [e, pl, t] : viewPointLights.each())
  {
    renderer.submitPointLight(static_cast<entt::id_type>(e), pl,
                              t.getGlobalPosition());
  }

  auto viewMeshes = m_Registry.view<MeshComponent, TransformComponent>();
  for (auto&& [e, m, t] : viewMeshes.each())
  {
    renderer.submitMesh(static_cast<uint32_t>(e), m, &t.getGlobalTransform());
  }

  auto viewSkyboxes = m_Registry.view<SkyboxComponent>();
  for (auto&& [e, skybox] : viewSkyboxes.each())
  {
    renderer.submitSkybox(skybox);
  }

  auto viewParticles =
    m_Registry.view<ParticleSystemComponent, TransformComponent>();
  for (auto&& [e, particleSystem, t] : viewParticles.each())
  {
    renderer.submitParticleSystem(static_cast<uint32_t>(e), particleSystem,
                                  &t.getGlobalTransform());
  }

  onRender();

  renderer.endScene();
}

void Scene::end()
{
  onEnd();

  m_Registry.clear(); // So that entities are destroyed properly
  m_renderer.reset();
}

void Scene::onWindowResized(const glm::uvec2& size)
{
  m_renderer->setFrameSize(size);
}

void Scene::setResizesWithWindow(bool resizesWithWindow)
{
  m_windowResizeBinder = Application::Get().getGameLayer().getCustomEvent(
    "VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE");

  if (resizesWithWindow)
  {
    m_windowResizeBinder.bindCallback(
      [this](const Event& e)
      {
        onWindowResized({ e.newWidth, e.newHeight });
      });
  }
}

void Scene::spawn()
{
  onSpawn();

  auto spawner = [](Entity& current, Entity& parent) -> bool
  {
    if (current.hasScriptComponent())
    {
      current.getScriptComponent().onSpawn();
    }

    return true;
  };

  DepthFirstTraversal<false>(spawner, getRoot());
  m_spawned = true;
}

void Scene::setSplitScreenGridSize(size_t rows, size_t columns)
{
  VRM_ASSERT_MSG(rows > 0 && columns > 0,
                 "At least 1 row and 1 column are needed");
  RenderLayout newLayout(rows, columns);

  size_t keptRows = std::min(rows, m_renderLayout.getRows());
  size_t keptCols = std::min(columns, m_renderLayout.getCols());

  for (size_t i = 0; i < keptCols; ++i)
  {
    for (size_t j = 0; j < keptRows; ++j)
    {
      newLayout.setView(j, i, m_renderLayout.getView(j, i));
    }
  }

  m_renderLayout = std::move(newLayout);
}

void Scene::setCamera(CameraBasic* camera)
{
  render::NormalizedView view(camera);
  m_renderLayout.setView(0, 0, view); // (0, 0) is always valid
}

void Scene::setCamera(CameraBasic* camera, size_t row, size_t col)
{
  VRM_ASSERT_MSG(row < m_renderLayout.getRows(),
                 "Row {} is out of bounds. Row count is {}", row,
                 m_renderLayout.getRows());
  VRM_ASSERT_MSG(col < m_renderLayout.getCols(),
                 "Column {} is out of bounds. Column count is {}", col,
                 m_renderLayout.getCols());

  render::NormalizedView view(camera);
  m_renderLayout.setView(row, col, view);
}

CameraBasic* Scene::getCamera() const
{
  return m_renderLayout.getView(0, 0).getCamera();
}

Entity Scene::createEntity(const std::string& nameTag)
{
  auto  e          = createRawEntity(nameTag);
  auto& hierarchy  = e.getComponentInternal<HierarchyComponent>();
  hierarchy.parent = m_Root;
  getRoot().getComponentInternal<HierarchyComponent>().children.emplace_back(
    e.clone());

  return e;
}

Entity Scene::createEntity()
{
  while (entityExists("NewEntity_" + std::to_string(m_EntityCounter)))
    ++m_EntityCounter;

  return createEntity("NewEntity_" + std::to_string(m_EntityCounter++));
}

void Scene::renameEntity(Entity& e, const std::string& name)
{
  VRM_ASSERT_MSG(e != m_Root, "You cannot rename root entity.");
  VRM_ASSERT_MSG(!name.empty(), "You cannot use an empty name.");
  VRM_ASSERT_MSG(
    !m_EntitiesByName.contains(name),
    "Another entity is already named \"{}\". Consider using entityExists(const "
    "std::string& name) before renaming an entity.",
    name);
  VRM_ASSERT_MSG(e.isValid(), "Entity is not valid.");
  m_EntitiesByName.erase(e.getName());
  m_EntitiesByName[name]                       = e.clone();
  e.getComponentInternal<NameComponent>().name = name;

  // @todo Keep entities sorted by name in HierarchyComponent
}

bool Scene::entityExists(const std::string& name) const
{
  return m_EntitiesByName.contains(name);
}

bool Scene::entityExists(entt::entity e) const { return m_Registry.valid(e); }

Entity Scene::getEntity(entt::entity handle)
{
  VRM_ASSERT_MSG(m_Registry.valid(handle), "Entity is not valid.");

  return Entity(handle, &m_Registry, this);
}

Entity Scene::getEntity(const std::string& name)
{
  VRM_ASSERT_MSG(entityExists(name),
                 "Entity with name " + name + " does not exist.");

  return m_EntitiesByName.at(name);
}

bool Scene::checkEntitiesRelation(const Entity& parent,
                                  const Entity& child) const
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  return child.getComponent<HierarchyComponent>().parent == parent;
}

bool Scene::checkEntityAncestor(const Entity& ancestor,
                                const Entity& child) const
{
  if (child.getParent() == ancestor) return true;
  if (child.isRoot()) return false;

  return checkEntityAncestor(ancestor, child.getParent());
}

void Scene::setEntitiesRelation(const Entity& parent, const Entity& child)
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  VRM_ASSERT_MSG(child != m_Root, "You cannot set Root node's parent");

  if (checkEntitiesRelation(parent, child)) return;

  auto& hParent   = parent.getComponentInternal<HierarchyComponent>();
  auto& hChild    = child.getComponentInternal<HierarchyComponent>();
  auto& hExParent = hChild.parent.getComponentInternal<HierarchyComponent>();

  hExParent.children.remove(child);
  hChild.parent = parent;
  hParent.children.emplace_back(child.clone());

  auto& tChild = child.getComponentInternal<vrm::TransformComponent>();
  TransformComponent::SceneAttorney::setFrameDirty(tChild, true);
}

void Scene::destroyEntity(const Entity& entity)
{
  VRM_ASSERT_MSG(m_Registry.valid(entity), "Entity is not valid.");
  VRM_ASSERT_MSG(entity != m_Root, "You cannot delete root entity!");

  auto parent = entity.getComponentInternal<HierarchyComponent>().parent;
  VRM_ASSERT_MSG(m_Registry.valid(parent.getHandle()),
                 "The parent of the entity you want to destroy is not valid");

  destroyEntityRecursive(entity);

  // Removing entity from its parent children
  parent.getComponentInternal<HierarchyComponent>().children.remove(entity);
}

void Scene::destroyEntityRecursive(Entity entity)
{
  auto& h        = entity.getComponentInternal<HierarchyComponent>();
  auto  children = std::move(h.children);

  for (auto child : children) { destroyEntityRecursive(child); }

  if (entity.hasScriptComponent()) { entity.getScriptComponent().onDestroy(); }

  m_EntitiesByName.erase(entity.getName());
  m_Registry.destroy(entity);
}

void Scene::destroyAllEntities()
{
  m_Registry.clear();
  m_EntitiesByName.clear();
  m_Root = createRoot();
}

Entity Scene::createRawEntity(const std::string& nameTag)
{
  VRM_ASSERT_MSG(!entityExists(nameTag),
                 "Entity with name " + nameTag + " already exists.");
  auto e = getEntity(m_Registry.create());
  e.addComponent<NameComponent>(nameTag);
  e.addComponent<TransformComponent>();
  auto& hierarchy           = e.addComponent<HierarchyComponent>();
  m_EntitiesByName[nameTag] = e;

  return e;
}

Entity Scene::createRoot()
{
  auto e                                              = createRawEntity("Root");
  e.getComponentInternal<HierarchyComponent>().parent = Entity();

  return e;
}

void Scene::renameRoot(const std::string& rootName)
{
  m_EntitiesByName.erase(m_Root.getName());
  m_EntitiesByName[rootName]                        = m_Root.clone();
  m_Root.getComponentInternal<NameComponent>().name = rootName;
}

void Scene::_updateGlobalTransforms()
{
  auto effector = [](Entity& current, Entity& parent) -> bool
  {
    auto& currentTC = current.getComponentInternal<vrm::TransformComponent>();

    TransformComponent::SceneAttorney::computeGlobals(
      currentTC, parent.getComponentInternal<vrm::TransformComponent>());

    TransformComponent::SceneAttorney::setFrameDirty(currentTC, false);

    return true;
  };

  auto checker = [this, &effector](Entity& current, Entity& parent) -> bool
  {
    auto& currentTC = current.getComponentInternal<vrm::TransformComponent>();

    if (currentTC.isFrameDirty())
    {
      DepthFirstTraversal<true>(effector, current);
      return false; // Checking aborted, everything deeper will be updated
    }

    return true;
  };

  DepthFirstTraversal<false>(checker, getRoot());
}
