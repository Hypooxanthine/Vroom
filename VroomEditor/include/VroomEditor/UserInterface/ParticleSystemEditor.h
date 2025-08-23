#pragma once

#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"
#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Scene.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterEditor.h"
#include "VroomEditor/UserInterface/ViewportModule.h"
#include <functional>
#include <vector>

namespace vrm::editor
{

  class ParticleSystemEditor : public ImGuiElement
  {
  private:

    struct MyViewportModule : public ViewportModule
    {
      std::function<void(const glm::uvec2&)> resizeCb;

    protected:
      void onPopupImgui(const ImVec2& texturePx) override;
      void onLeftClick(const ImVec2& texturePx) override;
      void onResize(const ImVec2& size) override;
    };

  public:

    ParticleSystemEditor();
    ~ParticleSystemEditor();

    ParticleSystemEditor& operator=(const ParticleSystemEditor& other) = delete;
    ParticleSystemEditor(const ParticleSystemEditor& other) = delete;

    ParticleSystemEditor& operator=(ParticleSystemEditor&& other) = delete;
    ParticleSystemEditor(ParticleSystemEditor&& other) = delete;

    void open(ParticleSystemAsset::Handle asset);
    void close();

  protected:

    void onUpdate(const DeltaTime &dt) override;
    void onRender() override;
    void onImgui() override;

  private:

    void _showSettings();
    void _showEmitterContextualMenu();

  private:

    MyViewportModule m_viewport;

    ParticleSystemAsset::Handle m_particlesAsset;
    Scene m_scene;
    FirstPersonCamera m_camera;
    Entity m_entity;

    std::vector<EmitterEditor> m_emitters;

  };

}