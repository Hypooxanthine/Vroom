#include "VroomEditor/UserInterface/ParticleSystem/EmitterEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"

using namespace vrm::editor;

EmitterEditor::EmitterEditor(const ParticleEmitter::Specs& initSpecs)
{
  _initAttributes(initSpecs);
}

EmitterEditor::~EmitterEditor()
{

}

void EmitterEditor::onImgui()
{
  for (EmitterAttributeEditor& attribute : m_attributes)
  {
    attribute.renderImgui();
  }
}

bool EmitterEditor::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  bool changed = false;

  for (const EmitterAttributeEditor& attribute : m_attributes)
  {
    changed = attribute.updateEmitterSpecs(specs) || changed;
  }

  return changed;
}

void EmitterEditor::_initAttributes(const ParticleEmitter::Specs& initSpecs)
{
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
