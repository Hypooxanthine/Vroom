#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "glm/fwd.hpp"
#include "imgui.h"

using namespace vrm;

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
  : ImGuiElement()
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  auto& psc = m_entity.addComponent<ParticleSystemComponent>();
  ParticleEmitterSpecs specs;
  ParticleEmitter emitter;
  emitter.setSpecs(specs);
  psc.addEmitter(std::move(emitter));

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
    m_viewport.renderImgui();
  }
  ImGui::End();
}
