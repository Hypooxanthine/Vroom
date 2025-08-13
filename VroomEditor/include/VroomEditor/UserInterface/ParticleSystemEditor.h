#pragma once

#include "Vroom/Asset/StaticAsset/ParticleSystemAsset.h"
#include "Vroom/Scene/Entity.h"
#include "Vroom/Scene/Scene.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"

namespace vrm
{

  class ParticleSystemEditor : public ImGuiElement
  {
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

  private:

    ParticleSystemAsset::Handle m_particlesAsset;
    Scene m_scene;
    Entity m_entity;

  };

}