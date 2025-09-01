#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Core/GameLayer.h"
#include "Vroom/Core/Layer.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterEditor.h"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <string>

using namespace vrm::editor;

void ParticleSystemEditor::MyViewportModule::onPopupImgui(const ImVec2& texturePx)
{

}

void ParticleSystemEditor::MyViewportModule::onLeftClick(const ImVec2& texturePx)
{

}

void ParticleSystemEditor::MyViewportModule::onResize(const ImVec2& size)
{
  resizeCb(glm::uvec2(size.x, size.y));
}

ParticleSystemEditor::ParticleSystemEditor()
  : ImGuiElement(), m_camera(0.1f, 100.f, 90.f, 1.f, glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f))
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  m_entity.addComponent<ParticleSystemComponent>();
  {
    _addEmitter();
  }

  m_scene.setCamera(&m_camera);

  m_scene.createEntity("Skybox").addComponent<SkyboxComponent>();

  {
    RenderSettings settings;
    settings.antiAliasingLevel = 1;
    settings.bloom.activated = false;
    settings.clusteredShading = false;
    settings.normalMapping.activated = false;
    settings.shadowsEnable = false;

    m_scene.getRenderer().getRenderPipeline().setRenderSettings(settings);
  }

  m_viewport.setSupportPopup(false);
  m_viewport.resizeCb = [this](const glm::uvec2& size)
  {
    m_scene.onWindowResized(size);
  };
}

ParticleSystemEditor::~ParticleSystemEditor()
{

}

void ParticleSystemEditor::open(ParticleSystemAsset::Handle asset)
{
  m_particlesAsset = asset;
  if (m_open)
  {
    *m_open = true;
  }
}

void ParticleSystemEditor::close()
{
  m_particlesAsset = {};
  if (m_open)
  {
    *m_open = false;
  }
}

void ParticleSystemEditor::onUpdate(const DeltaTime &dt)
{
  if (!m_open || *m_open)
  {
    m_scene.update(Application::Get().getDeltaTime());

    for (EmitterEditor& emitterEditor : m_emitters)
    {
      emitterEditor.update(dt);
    }

    _updateEmitterSpecs();
    _checkRemovedEmitter();
  }
}

void ParticleSystemEditor::_updateEmitterSpecs()
{
  for (size_t i = 0; i < m_emitters.size(); ++i)
  {
    bool specsChanged = false;
    ParticleEmitter& emitter = m_entity.getComponent<ParticleSystemComponent>().getEmitters()[i];
    const EmitterEditor& editor = m_emitters.at(i);
    ParticleEmitter::Specs specs = emitter.getSpecs();

    specsChanged = editor.updateEmitterSpecs(specs) || specsChanged;

    if (specsChanged)
    {
      emitter.setSpecs(specs);
    }
  }
}

void ParticleSystemEditor::_checkRemovedEmitter()
{
  size_t i;

  for (i = 0; i < m_emitters.size(); ++i)
  {
    auto& emitter = m_emitters.at(i);

    if (emitter.requestedDelete())
    {
      m_emitters.erase(m_emitters.begin() + i);
      m_entity.getComponent<ParticleSystemComponent>().removeEmitter(i);
    
      for (size_t i = 0; i < m_emitters.size(); ++i)
      {
        m_emitters.at(i).setName("Emitter " + std::to_string(i));
      }

      break;
    }
  }
}

void ParticleSystemEditor::onRender()
{
  if (!m_open || *m_open)
  {
    m_scene.render();
    m_viewport.setTexture(m_scene.getRenderer().getRenderPipeline().getRenderedTexture());
  }
}

void ParticleSystemEditor::onImgui()
{
  if (ImGui::Begin("Particle System", m_open))
  {
    if (ImGui::BeginTable("Table", 2, ImGuiTableFlags_Resizable))
    {
      ImGui::TableNextRow();

      if (ImGui::TableNextColumn())
      {
        m_viewport.renderImgui();
      }

      if (ImGui::TableNextColumn())
      {
        _showSettings();
      }

      ImGui::EndTable();
    }
  }
  ImGui::End();
}

void ParticleSystemEditor::_addEmitter()
{
  auto& psc = m_entity.getComponent<ParticleSystemComponent>();
  psc.addEmitter({});
  EmitterEditor& emitter = m_emitters.emplace_back();
  emitter.setName("Emitter " + std::to_string(m_emitters.size() - 1)); 
}

void ParticleSystemEditor::_showSettings()
{
  if (ImGui::BeginChild("Settings"))
  {
    for (EmitterEditor& emitter : m_emitters)
    {
      emitter.renderImgui();
    }

    if (ImGui::Button("Add emitter"))
    {
      Application::Get().getGameLayer().pushFrameEndRoutine([this](Layer& layer) {
        _addEmitter();
      });
    }
  }
  ImGui::EndChild();
}
