#include "Vroom/Scene/Scene.h"

#include "Vroom/Asset/Asset.h"
#include "Vroom/Render/Camera/CameraBasic.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"

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

} // namespace vrm
