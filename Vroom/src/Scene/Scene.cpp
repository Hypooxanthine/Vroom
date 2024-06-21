#include "Vroom/Scene/Scene.h"

#include "Vroom/Asset/Asset.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"
#include "Vroom/Scene/Entity.h"

#include "Vroom/Scene/Components/NameComponent.h"

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

}

void Scene::init(Application* app)
{
    m_Application = app;

    onInit();
}

void Scene::update(float dt)
{

    onUpdate(dt);
}

void Scene::render()
{

    onRender();
}

void Scene::end()
{

    onEnd();
}

Entity Scene::createEntity(const std::string& nameTag)
{
    VRM_ASSERT_MSG(!entityExists(nameTag), "Entity with name " + nameTag + " already exists.");
    auto e = getEntity(m_Registry.create());
    e.addComponent<NameComponent>(nameTag);

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
