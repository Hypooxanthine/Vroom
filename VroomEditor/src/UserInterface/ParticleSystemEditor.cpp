#include "VroomEditor/UserInterface/ParticleSystemEditor.h"
#include "Vroom/Scene/Components/ParticleSystemComponent.h"
#include "imgui.h"

using namespace vrm;

ParticleSystemEditor::ParticleSystemEditor()
  : ImGuiElement()
{
  m_scene.init();
  m_entity = m_scene.createEntity("Particles");
  m_entity.addComponent<ParticleSystemComponent>();
}

ParticleSystemEditor::~ParticleSystemEditor()
{

}

void ParticleSystemEditor::open(ParticleSystemAsset::Handle asset)
{
  m_particlesAsset = asset;
}

void ParticleSystemEditor::close()
{
  m_particlesAsset = {};
  if (m_open)
  {
    *m_open = false;
  }
}

bool ParticleSystemEditor::onImgui()
{

  if (ImGui::Begin("Particle System", m_open))
  {

  }
  ImGui::End();

  return true;
}