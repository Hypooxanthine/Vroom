#pragma once

#include "AssetManager/ParticleSystemAsset.h"
#include "Renderer/FirstPersonCamera.h"
#include "Renderer/ParticleEmitter.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Editor/ImGuiElement.h"
#include "Editor/EmitterEditor.h"
#include "Editor/ViewportModule.h"
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

    void _addEmitter();
    void _showSettings();

    void _updateEmitterSpecs();
    void _checkRemovedEmitter();

  private:

    MyViewportModule m_viewport;

    ParticleSystemAsset::Handle m_particlesAsset;
    Scene m_scene;
    FirstPersonCamera m_camera;
    Entity m_entity;

    std::vector<EmitterEditor> m_emitters;

  };

}