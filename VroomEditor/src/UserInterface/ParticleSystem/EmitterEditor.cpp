#include "VroomEditor/UserInterface/ParticleSystem/EmitterEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterEditor::EmitterEditor(const ParticleEmitter::Specs& initSpecs)
{
  _initAttributes(initSpecs);
}

EmitterEditor::~EmitterEditor()
{

}

void EmitterEditor::setName(const std::string& name)
{
  m_name = name;
}

bool EmitterEditor::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  bool changed = m_changed;

  if (m_changed)
  {
    specs.emitRate = m_emitRate;
    specs.lifeTime.getRawData()[0] = m_lifeTime;
    
    m_changed = false;
  }

  for (const EmitterAttributeEditor& attribute : m_attributes)
  {
    changed = attribute.updateEmitterSpecs(specs) || changed;
  }

  return changed;
}

void EmitterEditor::onImgui()
{
  if (ImGui::TreeNode(m_name.c_str()))
  {
    ImGui::TreePush("Life cycle");
    {
      m_changed = ImGui::SliderFloat("Emit rate", &m_emitRate, 0.1f, 500.f);
      m_changed = ImGui::SliderFloat("Life time", &m_lifeTime, 0.1f, 20.f);

      ImGui::TreePop();
    }

    for (EmitterAttributeEditor& attribute : m_attributes)
    {
      attribute.renderImgui();
    }

    ImGui::TreePop();
  }
}

void EmitterEditor::_showContextualMenu()
{
  
}

void EmitterEditor::_initAttributes(const ParticleEmitter::Specs& initSpecs)
{
  m_emitRate = initSpecs.emitRate;
  m_lifeTime = initSpecs.lifeTime;

  EmitterAttributeEditor::Settings settings;
  settings.type = EmitterFieldEditor::EType::eVec3;
  settings.min = -10.f;
  settings.max = 10.f;

  _addAttribute(initSpecs, settings, "Position", ParticleEmitter::Specs::EAttributeName::ePosition);

  settings.min = 0.f;
  settings.max = 10.f;
  _addAttribute(initSpecs, settings, "Scale", ParticleEmitter::Specs::EAttributeName::eScale);

  settings.type = EmitterFieldEditor::EType::eColor4;
  settings.min = 0.f;
  settings.max = 1.f;
  _addAttribute(initSpecs, settings, "Color", ParticleEmitter::Specs::EAttributeName::eColor);
}

void EmitterEditor::_addAttribute(const ParticleEmitter::Specs& initSpecs, const EmitterAttributeEditor::Settings& settings,  const std::string& displayName, ParticleEmitter::Specs::EAttributeName name)
{
  auto& attr = m_attributes.emplace_back(displayName, name);
  attr.setSettings(settings);

  attr.setValue(initSpecs.getAttribute(name));
}
