#include "VroomEditor/UserInterface/ParticleSystem/ParticleAttribute.h"
#include "VroomEditor/UserInterface/ParticleSystem/ParticleField.h"
#include "imgui.h"

using namespace vrm;

ParticleAttribute::ParticleAttribute(const std::string& displayName, ParticleEmitter::Specs::EAttributeName name)
{
  m_name = name;
  setName(displayName);
  m_spawnValue.setName("Spawn");
  m_deathValue.setName("Death");
}

ParticleAttribute::~ParticleAttribute()
{

}

void ParticleAttribute::setValue(const ParticleEmitterAttributeBase& emitterAttribute)
{
  m_spawnValue.setValue(emitterAttribute.getSpawnFieldBase().getRawData());
  m_deathValue.setValue(emitterAttribute.getDeathFieldBase().getRawData());
}

bool ParticleAttribute::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  return m_spawnValue.updateEmitterField(specs.getAttribute(m_name).getSpawnFieldBase())
  || m_deathValue.updateEmitterField(specs.getAttribute(m_name).getDeathFieldBase());
}

void ParticleAttribute::onImgui()
{
  if (ImGui::TreeNode(m_displayName.c_str()))
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
  m_displayName = name;
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
