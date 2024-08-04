#include "VroomEditor/EditorLayer.h"

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>
#include <Vroom/Core/Window.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

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

    // Engine setup
    Application::Get().getGameLayer().getFrameBuffer().setOnScreenRender(false);

    // Frame buffer
    FrameBuffer::Specification specs = {
        .onScreen = true,
        .width = Application::Get().getWindow().getWidth(),
        .height = Application::Get().getWindow().getHeight(),
        .useBlending = true,
        .useDepthTest = true,
        .clearColor = {0.1f, 0.1f, 0.1f, 1.0f}
    };
    m_FrameBuffer.create(specs);
    m_FrameBuffer.bind();

    // Imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(Application::Get().getWindow().getGLFWHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void EditorLayer::onEnd()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void EditorLayer::onUpdate(float dt)
{
}

void EditorLayer::onRender()
{
    m_FrameBuffer.bind();
    m_FrameBuffer.clearColorBuffer();

    onImgui();
}

void EditorLayer::onEvent(vrm::Event& e)
{
    m_CustomEventManager.check(e);
}

void EditorLayer::onImgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    
    m_MainMenuBar.renderImgui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace vrm
