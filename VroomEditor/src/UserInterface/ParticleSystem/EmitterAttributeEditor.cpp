#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterAttributeEditor::EmitterAttributeEditor(const std::string& displayName, ParticleEmitterAttributeBase::EAttributeName name)
   : m_name(name), m_displayName(displayName)
{
}

EmitterAttributeEditor::~EmitterAttributeEditor()
{

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

void EmitterAttributeEditor::onUpdate(const DeltaTime& dt)
{
  if (m_spawnField->requestedNewFieldType())
    assignSpawnField(EmitterFieldEditor::Instanciate(m_spawnField->getNextType()));

  if (m_deathField->requestedNewFieldType())
    assignDeathField(EmitterFieldEditor::Instanciate(m_deathField->getNextType()));
}

void EmitterAttributeEditor::assignField(std::unique_ptr<EmitterFieldEditor>& field, EmitterFieldEditor* newField, const EmitterScalarEditor::Settings& settings)
{
  field.reset(newField);
  field->setScalarSettings(settings);
}

EmitterPositionEditor::EmitterPositionEditor()
  : EmitterAttributeEditor("Position", ParticleEmitterAttributeBase::EAttributeName::ePosition)
{
  m_spawnFieldSettings.scalarType = EmitterScalarEditor::EScalarType::eVec3;
  m_spawnFieldSettings.minValue = { -10.f, -10.f, -10.f };
  m_spawnFieldSettings.maxValue = { 10.f, 10.f, 10.f };
  m_spawnFieldSettings.defaultValue = { 0.f, 0.f, 0.f };
  m_spawnFieldSettings.scaleLocked = false;

  m_deathFieldSettings = m_spawnFieldSettings;
  m_deathFieldSettings.defaultValue = { 0.f, 5.f, 0.f };

  assignSpawnField(new ConstEmitterFieldEditor());
  assignDeathField(new ConstEmitterFieldEditor());
}

EmitterPositionEditor::~EmitterPositionEditor()
{

}

EmitterScaleEditor::EmitterScaleEditor()
  : EmitterAttributeEditor("Scale", ParticleEmitterAttributeBase::EAttributeName::eScale)
{
  m_spawnFieldSettings.scalarType = EmitterScalarEditor::EScalarType::eVec3;
  m_spawnFieldSettings.minValue = { 0.1f, 0.1f, 0.1f };
  m_spawnFieldSettings.maxValue = { 5.f, 5.f, 5.f };
  m_spawnFieldSettings.defaultValue = { 1.f, 1.f, 1.f };
  m_spawnFieldSettings.scaleLocked = true;

  m_deathFieldSettings = m_spawnFieldSettings;

  assignSpawnField(new ConstEmitterFieldEditor());
  assignDeathField(new ConstEmitterFieldEditor());
}

EmitterScaleEditor::~EmitterScaleEditor()
{
  
}

EmitterColorEditor::EmitterColorEditor()
  : EmitterAttributeEditor("Color", ParticleEmitterAttributeBase::EAttributeName::eColor)
{
  m_spawnFieldSettings.scalarType = EmitterScalarEditor::EScalarType::eColor4;
  m_spawnFieldSettings.minValue = { 0.f, 0.f, 0.f, 0.f };
  m_spawnFieldSettings.maxValue = { 1.f, 1.f, 1.f, 1.f };
  m_spawnFieldSettings.defaultValue = { 1.f, 0.f, 0.f, 1.f };
  m_spawnFieldSettings.scaleLocked = false;

  m_deathFieldSettings = m_spawnFieldSettings;
  m_deathFieldSettings.defaultValue = { 0.f, 0.f, 1.f, 1.f };

  assignSpawnField(new ConstEmitterFieldEditor());
  assignDeathField(new ConstEmitterFieldEditor());
}

EmitterColorEditor::~EmitterColorEditor()
{
  
}
