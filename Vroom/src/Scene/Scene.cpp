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

namespace vrm
{

Scene::Scene()
{
    // Setting a default camera
    setCamera(&m_DefaultCamera);
}

Scene::~Scene()
{

}

void Scene::init(Application* app)
{
    m_Application = app;

    getApplication().getGameLayer().getCustomEvent("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE")
        .bindCallback([this](const vrm::Event& e) {
            getCamera().setViewportSize(static_cast<float>(e.newWidth), static_cast<float>(e.newHeight));
        });

    onInit();

    const auto& viewportSize = getApplication().getRenderer().getViewportSize();
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
    Application& app = getApplication();
    Renderer& renderer = app.getRenderer();
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

AssetManager& Scene::getAssetManager()
{
    return getApplication().getAssetManager();
}

const AssetManager& Scene::getAssetManager() const
{
    return getApplicaton().getAssetManager();
}

Entity Scene::createEntity(const std::string& nameTag)
{
    VRM_ASSERT_MSG(!entityExists(nameTag), "Entity with name " + nameTag + " already exists.");
    auto e = getEntity(m_Registry.create());
    e.addComponent<NameComponent>(nameTag);
    e.addComponent<TransformComponent>();

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

    return Entity(handle, &m_Registry);
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

void Scene::destroyEntity(Entity entity)
{
    VRM_ASSERT_MSG(m_Registry.valid(entity), "Entity is not valid.");

    if(entity.hasComponent<ScriptHandler>())
    {
        entity.getComponent<ScriptHandler>().getScript().onDestroy();
    }

    m_Registry.destroy(entity);
}

} // namespace vrm
