#include "VroomEditor/UserInterface/ParticleSystem/ParticleAttribute.h"
#include "VroomEditor/UserInterface/ParticleSystem/ParticleField.h"
#include "imgui.h"

using namespace vrm;

ParticleAttribute::ParticleAttribute(const std::string& name)
{
  setName(name);
  m_spawnValue.setName("Spawn");
  m_deathValue.setName("Death");
}

ParticleAttribute::~ParticleAttribute()
{

}

bool ParticleAttribute::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  return false;
}

void ParticleAttribute::onImgui()
{
  if (ImGui::TreeNode(m_name.c_str()))
  {
    ImGui::PushID("spawn");
    {
      m_spawnValue.renderImgui();
      ImGui::PopID();
    }

    ImGui::PushID("death");
    {
      m_deathValue.renderImgui();
      ImGui::PopID();
    }

    ImGui::TreePop();
  }
}

void ParticleAttribute::setName(const std::string& name)
{
  m_name = name;
}

void ParticleAttribute::setSpecs(const Specs& specs)
{
  auto setup = [this, &specs](ParticleField& field)
  {
    field.setType(specs.type);
    field.setBounds(std::vector(field.getDimension(), specs.min), std::vector(field.getDimension(), specs.max));
    field.setLockScale(specs.scaleLocked);
  };
  
  setup(m_spawnValue);
  setup(m_deathValue);
}
