#pragma once

#include <Vroom/Core/Layer.h>
#include <Vroom/Render/Abstraction/FrameBuffer.h>
#include <Vroom/Event/CustomEvent/CustomEventManager.h>

#include "VroomEditor/UserInterface/MainMenuBar.h"

namespace vrm
{

class EditorLayer : public vrm::Layer
{
public:
    EditorLayer();
    ~EditorLayer();

protected:

    virtual void onInit();
    virtual void onEnd();
    virtual void onUpdate(float dt);
    virtual void onRender();
    virtual void onEvent(vrm::Event& e);

private:
    void onImgui();

private:
    FrameBuffer m_FrameBuffer;
    CustomEventManager m_CustomEventManager;

    // UI
    MainMenuBar m_MainMenuBar;

};

} // namespace vrm