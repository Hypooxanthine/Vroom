#pragma once

#include <Vroom/Core/Layer.h>
#include <Vroom/Render/Abstraction/FrameBuffer.h>
#include <Vroom/Event/CustomEvent/CustomEventManager.h>
#include <Vroom/Event/Trigger/TriggerManager.h>
#include <Vroom/Render/Camera/FirstPersonCamera.h>

#include "VroomEditor/UserInterface/MainMenuBar.h"
#include "VroomEditor/UserInterface/StatisticsPanel.h"
#include "VroomEditor/UserInterface/Viewport.h"
#include "VroomEditor/UserInterface/AssetBrowser.h"
#include "VroomEditor/EditorCamera.h"

struct ImFont;

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

    void onViewportResize();

private:
    FrameBuffer m_FrameBuffer;
    CustomEventManager m_CustomEventManager;
    TriggerManager m_TriggerManager;
    ImFont* m_Font;

    // UI
    MainMenuBar m_MainMenuBar;
    StatisticsPanel m_StatisticsPanel;
    Viewport m_Viewport;
    AssetBrowser m_AssetBrowser;

    // Frame time management
    size_t m_FrameAccumulator;
    float m_TimeAccumulator;
    const float m_TimeSample;

    EditorCamera m_EditorCamera;
};

} // namespace vrm