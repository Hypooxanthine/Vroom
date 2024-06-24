#include "Vroom/Scene/Scene.h"

#include "Vroom/Core/Application.h"

#include "Vroom/Asset/Asset.h"

#include "Vroom/Render/Renderer.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"

#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Components/NameComponent.h"
#include "Vroom/Scene/Components/TransformComponent.h"
#include "Vroom/Scene/Components/MeshComponent.h"

namespace vrm
{

Scene::Scene()
    : m_AssetManager(nullptr)
{
    // Instanciating asset manager
    m_AssetManager = std::make_unique<AssetManager>();

    // Setting a default camera
    setCamera(&m_DefaultCamera);
}

Scene::~Scene()
{
    m_AssetManager.release();
}

void Scene::init(Application* app)
{
    m_Application = app;

    getApplication().createCustomEvent("VRM_RESERVED_CUSTOM_EVENT_WINDOW_RESIZE")
        .bindInput(vrm::Event::Type::WindowsResized)
        .bindCallback([this](const vrm::Event& e) {
            getApplication().getRenderer().setViewport({ 0.f, 0.f}, { static_cast<float>(e.newWidth), static_cast<float>(e.newHeight) });
        })
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
}

void Scene::render()
{
    getApplication().getRenderer().beginScene(getCamera());

    const auto& renderer = m_Application->getRenderer();

    auto view = m_Registry.view<MeshComponent, TransformComponent>();
    for (auto entity : view)
    {
        auto& meshComponent = view.get<MeshComponent>(entity);
        auto& transformComponent = view.get<TransformComponent>(entity);

        renderer.drawMesh(meshComponent.getMesh(), transformComponent.getTransform());
    }

    onRender();

    getApplication().getRenderer().endScene();
}

void Scene::end()
{
    onEnd();

    m_Registry.clear(); // So that entities are destroyed properly
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
    m_Registry.destroy(entity);
}

} // namespace vrm
