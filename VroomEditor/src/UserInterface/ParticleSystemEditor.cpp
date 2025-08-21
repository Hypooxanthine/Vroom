#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "VroomEditor/UserInterface/ParticleSystem/ParticleAttribute.h"
#include "VroomEditor/UserInterface/ParticleSystem/ParticleField.h"
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
  : ImGuiElement(), m_camera(0.1f, 100.f, 90.f, 1.f, glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f))
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  auto& psc = m_entity.addComponent<ParticleSystemComponent>();
  {
    ParticleEmitter::Specs specs;
    specs.emitRate = 1.f;
    specs.lifeTime = 2.f;
    specs.color = { glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec4(0.f, 1.f, 0.f, 1.f) };
    specs.position = { glm::vec3(0.f), glm::vec3(0.f, 5.f, 0.f) };
    specs.scale = { glm::vec3(1.f) };

    psc.addEmitter(specs);
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

  m_viewport.resizeCb = [this](const glm::uvec2& size)
  {
    m_scene.onWindowResized(size);
  };

  _initAttributes();
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

void ParticleSystemEditor::_showSettings()
{
  if (ImGui::BeginChild("Settings"))
  {
    bool specsChanged = false;

    ParticleEmitter::Specs specs = m_entity.getComponent<ParticleSystemComponent>().getEmitters()[0].getSpecs();

    specsChanged = specsChanged || ImGui::SliderFloat("Emit rate", &specs.emitRate, 0.1f, 20.f);
    specsChanged = specsChanged || ImGui::SliderFloat("Life time", specs.lifeTime.getRawData().data(), 0.1f, 10.f);

    for (ParticleAttribute& attribute : m_attributes)
    {
      attribute.renderImgui();
    }

    for (const ParticleAttribute& attribute : m_attributes)
    {
      specsChanged = specsChanged || attribute.updateEmitterSpecs(specs);
    }

    if (specsChanged)
    {
      m_entity.getComponent<ParticleSystemComponent>().getEmitters()[0].setSpecs(specs);
    }
  }
  ImGui::EndChild();
}

void ParticleSystemEditor::_initAttributes()
{
  ParticleAttribute::Specs specs;
  specs.type = ParticleField::EType::eVec3;
  specs.min = -10.f;
  specs.max = 10.f;

  _addAttribute(specs, "Position", ParticleEmitter::Specs::EAttributeName::ePosition);

  specs.min = 0.f;
  specs.max = 10.f;
  _addAttribute(specs, "Scale", ParticleEmitter::Specs::EAttributeName::eScale);

  specs.type = ParticleField::EType::eColor4;
  specs.min = 0.f;
  specs.max = 1.f;
  _addAttribute(specs, "Color", ParticleEmitter::Specs::EAttributeName::eColor);
}

void ParticleSystemEditor::_addAttribute(const ParticleAttribute::Specs& specs,  const std::string& displayName, ParticleEmitter::Specs::EAttributeName name)
{
  auto& attr = m_attributes.emplace_back(displayName, name);
  attr.setSpecs(specs);

  const ParticleEmitter::Specs& emitterSpecs = m_entity.getComponent<ParticleSystemComponent>().getEmitters()[0].getSpecs();
  attr.setValue(emitterSpecs.getAttribute(name));
}
