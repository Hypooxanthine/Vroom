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

namespace vrm
{

Scene::Scene()
{
    m_Root = createRoot();
    getRoot().getComponent<HierarchyComponent>().parent = Entity();
    
    // Setting a default camera
    setCamera(&s_DefaultCamera);
}

Scene::~Scene()
{

}

void Scene::init()
{
    Application::Get().getGameLayer().getCustomEvent("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE")
        .bindCallback([this](const vrm::Event& e) {
            getCamera().setViewportSize(static_cast<float>(e.newWidth), static_cast<float>(e.newHeight));
        });

    onInit();

    const auto& viewportSize = Renderer::Get().getViewportSize();
    getCamera().setViewportSize(static_cast<float>(viewportSize.x), static_cast<float>(viewportSize.y));
}

void Scene::update(float dt)
{
    onUpdate(dt);

    auto viewScripts = m_Registry.view<ScriptHandler>();
    for (auto entity : viewScripts)
    {
        auto& scriptHandler = viewScripts.get<ScriptHandler>(entity);
        scriptHandler.getScript().onUpdate(dt);
    }
}

void Scene::render()
{
    Application& app = Application::Get();
    Renderer& renderer = Renderer::Get();
    renderer.beginScene(getCamera());
    
    auto viewPointLights = m_Registry.view<PointLightComponent, TransformComponent, NameComponent>();
    for (auto entity : viewPointLights)
    {
        const auto& pointLightComponent = viewPointLights.get<PointLightComponent>(entity);
        const auto& transformComponent = viewPointLights.get<TransformComponent>(entity);
        const auto& nameComponent = viewPointLights.get<NameComponent>(entity);

        renderer.submitPointLight(transformComponent.getPosition(), pointLightComponent, nameComponent.name);
    }

    auto viewMeshes = m_Registry.view<MeshComponent, TransformComponent>();
    for (auto entity : viewMeshes)
    {
        const auto& meshComponent = viewMeshes.get<MeshComponent>(entity);
        if (meshComponent.isVisible() == false)
            continue;
        const auto& transformComponent = viewMeshes.get<TransformComponent>(entity);

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

Entity Scene::createEntity(const std::string& nameTag)
{
    auto e = createRawEntity(nameTag);
    auto& hierarchy = e.getComponent<HierarchyComponent>();
    hierarchy.parent = getRoot();
    getRoot().getComponent<HierarchyComponent>().children.emplace(e);

    return e;
}

Entity Scene::createEntity()
{
    return createEntity("Entity_" + std::to_string(m_EntityCounter++));
}

bool Scene::entityExists(const std::string& name)
{
    auto view = m_Registry.view<NameComponent>();
    for (auto entity : view)
    {
        auto& nameComponent = view.get<NameComponent>(entity);
        if (nameComponent.name == name)
        {
            return true;
        }
    }

    return false;
}

Entity Scene::getEntity(entt::entity handle)
{
    VRM_ASSERT_MSG(m_Registry.valid(handle), "Entity is not valid.");

    return Entity(handle, &m_Registry, this);
}

Entity Scene::getEntity(const std::string& name)
{
    auto view = m_Registry.view<NameComponent>();
    for (auto entity : view)
    {
        auto& nameComponent = view.get<NameComponent>(entity);
        if (nameComponent.name == name)
        {
            return getEntity(entity);
        }
    }

    VRM_ASSERT_MSG(false, "Entity with name " + name + " does not exist.");
}

bool Scene::checkEntitiesRelation(Entity parent, Entity child) const
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  return child.getComponent<HierarchyComponent>().parent == parent;
}

void Scene::setEntitiesRelation(Entity parent, Entity child)
{
  VRM_ASSERT_MSG(m_Registry.valid(parent), "Unknown parent entity");
  VRM_ASSERT_MSG(m_Registry.valid(child), "Unknown child entity");

  VRM_ASSERT_MSG(child != m_Root, "You cannot set Root node's parent");
  
  if (checkEntitiesRelation(parent, child))
    return;
  
  auto& hParent = parent.getComponent<HierarchyComponent>();
  auto& hChild = child.getComponent<HierarchyComponent>();
  auto& hExParent = hChild.parent.getComponent<HierarchyComponent>();

  hExParent.children.erase(child);
  hChild.parent = parent;
  hParent.children.emplace(child);
}

void Scene::destroyEntity(Entity entity)
{
    VRM_ASSERT_MSG(m_Registry.valid(entity), "Entity is not valid.");
    VRM_ASSERT_MSG(entity != m_Root, "You cannot delete root entity!");

    auto parent = entity.getComponent<HierarchyComponent>().parent;

    destroyEntityRecursive(entity);

    // Removing entity from its parent children
    parent.getComponent<HierarchyComponent>().children.erase(entity);
}

void Scene::destroyEntityRecursive(Entity entity)
{
  if(entity.hasComponent<ScriptHandler>())
  {
      entity.getComponent<ScriptHandler>().getScript().onDestroy();
  }

  auto& h = entity.getComponent<HierarchyComponent>();
  auto children = std::move(h.children);

  m_Registry.destroy(entity);

  for (auto child : children)
  {
    destroyEntityRecursive(child);
  }
}

void Scene::destroyAllEntities()
{
  m_Registry.clear();
  m_Root = createRoot();
}

Entity Scene::createRawEntity(const std::string& nameTag)
{
  VRM_ASSERT_MSG(!entityExists(nameTag), "Entity with name " + nameTag + " already exists.");
  auto e = getEntity(m_Registry.create());
  e.addComponent<NameComponent>(nameTag);
  e.addComponent<TransformComponent>();
  auto& hierarchy = e.addComponent<HierarchyComponent>();

  return e;
}

Entity Scene::createRoot()
{
  auto e = createRawEntity("Root");
  e.getComponent<HierarchyComponent>().parent = Entity();
  
  return e;
}

} // namespace vrm
