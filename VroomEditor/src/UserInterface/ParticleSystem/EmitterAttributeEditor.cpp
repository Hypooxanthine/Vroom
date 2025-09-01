#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterAttributeEditor::EmitterAttributeEditor(const std::string& displayName, ParticleEmitter::Specs::EAttributeName name)
   : m_name(name), m_displayName(displayName)
{
}

EmitterAttributeEditor::~EmitterAttributeEditor()
{

}

void EmitterAttributeEditor::setValueFromSpecs(const ParticleEmitter::Specs& specs)
{
  m_spawnField->setData(specs.getAttribute(m_name).getSpawnFieldBase());
  m_deathField->setData(specs.getAttribute(m_name).getDeathFieldBase());
}

bool EmitterAttributeEditor::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  bool changed = false;
  
  changed = m_spawnField->updateEmitterField(specs.getAttribute(m_name).getSpawnFieldBase()) || changed;
  changed = m_deathField->updateEmitterField(specs.getAttribute(m_name).getDeathFieldBase()) || changed;

  return changed;
}

void EmitterAttributeEditor::onImgui()
{
  if (ImGui::TreeNode(m_displayName.c_str()))
  {
    ImGui::PushID("spawn");
    {
      m_spawnField->renderImgui();
      ImGui::PopID();
    }

    ImGui::PushID("death");
    {
      m_deathField->renderImgui();
      ImGui::PopID();
    }

    ImGui::TreePop();
  }
}

EmitterPositionEditor::EmitterPositionEditor()
  : EmitterAttributeEditor("Position", ParticleEmitter::Specs::EAttributeName::ePosition)
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue = { -10.f, -10.f, -10.f };
  settings.maxValue = { 10.f, 10.f, 10.f };
  settings.scaleLocked = false;

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Spawn");

    field->setScalarSettings(settings);
    m_spawnField.reset(field);
  }

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Death");

    field->setScalarSettings(settings);
    m_deathField.reset(field);
  }
}

EmitterPositionEditor::~EmitterPositionEditor()
{

}

EmitterScaleEditor::EmitterScaleEditor()
  : EmitterAttributeEditor("Scale", ParticleEmitter::Specs::EAttributeName::eScale)
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue = { -10.f, -10.f, -10.f };
  settings.maxValue = { 10.f, 10.f, 10.f };
  settings.scaleLocked = true;

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Spawn");

    field->setScalarSettings(settings);
    m_spawnField.reset(field);
  }

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Death");

    field->setScalarSettings(settings);
    m_deathField.reset(field);
  }
}

EmitterScaleEditor::~EmitterScaleEditor()
{
  
}

EmitterColorEditor::EmitterColorEditor()
  : EmitterAttributeEditor("Color", ParticleEmitter::Specs::EAttributeName::eColor)
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType = EmitterScalarEditor::EScalarType::eColor4;
  settings.minValue = { 0.f, 0.f, 0.f, 0.f };
  settings.maxValue = { 1.f, 1.f, 1.f, 1.f };
  settings.scaleLocked = false;

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Spawn");

    field->setScalarSettings(settings);
    m_spawnField.reset(field);
  }

  {
    auto* field = new ConstEmitterFieldEditor();
    field->setName("Death");

    field->setScalarSettings(settings);
    m_deathField.reset(field);
  }
}

EmitterColorEditor::~EmitterColorEditor()
{
  
}
