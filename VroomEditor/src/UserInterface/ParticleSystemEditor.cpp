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
  : ImGuiElement(), m_camera(0.1f, 100.f, 90.f, 1.f, glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f))
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  auto& psc = m_entity.addComponent<ParticleSystemComponent>();
  {
    ParticleEmitterSpecs specs;
    specs.emitRate = 1.f;
    specs.lifeTime = 10.f;
    specs.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    specs.acceleration = glm::vec3(0.f, -.5f, 0.f);
    specs.initialPosition = glm::vec3(0.f);
    specs.initialVelocity = glm::vec3(0.f, 3.f, 0.f);
    specs.scaleOverTime = glm::vec3(0.f);
    specs.initialScale = glm::vec3(1.f);

    ParticleEmitter emitter;
    emitter.setSpecs(specs);
    psc.addEmitter(std::move(emitter));
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
    ParticleEmitterSpecs specs = m_entity.getComponent<ParticleSystemComponent>().getEmitters()[0].getSpecs();
    auto updateSpecs = [this, &specs]() { m_entity.getComponent<ParticleSystemComponent>().getEmitters()[0].setSpecs(specs); };

    ImGuiSliderFlags sliderFlags = 0
      | ImGuiSliderFlags_AlwaysClamp
      | ImGuiSliderFlags_Logarithmic
    ;

    if (ImGui::SliderFloat("Lifetime", &specs.lifeTime, 0.01f, 15.f, "%.2f", sliderFlags))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat("Emit rate", &specs.emitRate, 0.1f, 100.f, "%.2f", sliderFlags))
    {
      updateSpecs();
    }

    if (ImGui::ColorPicker4("Color", &specs.color[0]))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat3("Initial position", &specs.initialPosition[0], -5.f, 5.f))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat3("Initial velocity", &specs.initialVelocity[0], -5.f, 5.f))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat3("Acceleration", &specs.acceleration[0], -5.f, 5.f))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat3("Initial scale", &specs.initialScale[0], 0.01f, 5.f))
    {
      updateSpecs();
    }

    if (ImGui::SliderFloat3("Scale over time", &specs.scaleOverTime[0], -5.f, 5.f))
    {
      updateSpecs();
    }
  }
  ImGui::EndChild();
}
