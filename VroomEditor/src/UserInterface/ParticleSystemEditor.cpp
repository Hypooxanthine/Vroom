#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "Vroom/Core/Application.h"
#include "Vroom/Render/Renderer.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "Vroom/Scene/Components/SkyboxComponent.h"
#include "imgui.h"

using namespace vrm;

ParticleSystemEditor::ParticleSystemEditor()
  : ImGuiElement()
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  m_entity.addComponent<ParticleSystemComponent>();

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
  m_scene.update(Application::Get().getDeltaTime());
}

void ParticleSystemEditor::onRender()
{
  m_scene.render();
}

bool ParticleSystemEditor::onImgui()
{
  if (ImGui::Begin("Particle System", m_open))
  {
    if (ImGui::BeginChild("ViewportFrame", ImVec2(0, 0), 0, ImGuiWindowFlags_NoScrollbar))
    {
      auto cursor = ImGui::GetCursorPos();
      auto size = ImGui::GetContentRegionAvail();

      m_scene.getRenderer().setFrameSize(glm::uvec2{ size.x, size.y });

      const gl::Texture* renderTexture = m_scene.getRenderer().getRenderPipeline().getRenderedTexture();

      if (renderTexture)
      {
        ImTextureID textureID = (ImTextureID)(intptr_t)renderTexture->getRenderId();
        ImVec2 imageSize = ImVec2(
          static_cast<float>(renderTexture->getDescription().width),
          static_cast<float>(renderTexture->getDescription().height));
        ImGui::Image(textureID, imageSize, ImVec2(0, 1), ImVec2(1, 0));
        ImVec2 rectMin = ImGui::GetItemRectMin();
        ImVec2 rectSize = ImGui::GetItemRectSize();
      }

    }
    ImGui::EndChild();
  }
  ImGui::End();

  return true;
}
