#include "VroomEditor/EditorLayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>

#include "VroomEditor/EditorScene.h"

namespace vrm
{


EditorLayer::EditorLayer()
{
    // We need to load a first scene before initialization of layers, because game layer will be initialized first.
    Application::Get().getGameLayer().loadScene<EditorScene>();
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::onInit()
{
    // Events setup
    m_CustomEventManager.createCustomEvent("Exit")
        .bindInput(Event::Type::Exit)
        .bindCallback([](const Event& e) {
            Application::Get().exit();
        });
}

void EditorLayer::onEnd()
{
}

void EditorLayer::onUpdate(float dt)
{
}

void EditorLayer::onRender()
{
}

void EditorLayer::onEvent(vrm::Event& e)
{
    m_CustomEventManager.check(e);
}

} // namespace vrm
