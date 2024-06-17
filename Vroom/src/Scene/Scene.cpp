#include "Vroom/Scene/Scene.h"

namespace vrm
{

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::init()
{

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
