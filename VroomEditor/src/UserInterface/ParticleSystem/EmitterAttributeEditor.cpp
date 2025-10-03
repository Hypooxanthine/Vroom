#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"

#include "imgui.h"

#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

using namespace vrm::editor;

EmitterAttributeEditor::EmitterAttributeEditor(const std::string& displayName)
  : m_displayName(displayName)
{}

EmitterAttributeEditor::~EmitterAttributeEditor() {}

void EmitterAttributeEditor::onImgui()
{
  if (ImGui::TreeNode(m_displayName.c_str()))
  {
    m_field->renderImgui();

    ImGui::TreePop();
  }
}

void EmitterAttributeEditor::onUpdate(const DeltaTime& dt)
{
  if (m_field->requestedNewFieldType())
    assignField(EmitterFieldEditor::Instanciate(m_field->getNextType()),
                m_scalarSettings);
}

void EmitterAttributeEditor::assignField(
  EmitterFieldEditor* newField, const EmitterScalarEditor::Settings& settings)
{
  m_field.reset(newField);
  m_field->setScalarSettings(settings);
  m_scalarSettings = settings;
}

EmitterSpawnPositionEditor::EmitterSpawnPositionEditor()
  : EmitterAttributeEditor("Spawn position")
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType   = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue     = { -10.f, -10.f, -10.f };
  settings.maxValue     = { 10.f, 10.f, 10.f };
  settings.defaultValue = { 0.f, 0.f, 0.f };
  settings.scaleLocked  = false;

  assignField(new ConstEmitterFieldEditor(), settings);
}

EmitterSpawnPositionEditor::~EmitterSpawnPositionEditor() {}

bool EmitterSpawnPositionEditor::updateEmitterSpecs(
  ParticleEmitter::Specs& specs) const
{
  bool changed = false;

  changed =
    m_field->updateEmitterField(specs.spawnPosition->getPositionFieldPtr())
    || changed;

  return changed;
}

EmitterSpawnVelocityEditor::EmitterSpawnVelocityEditor()
  : EmitterAttributeEditor("Spawn velocity")
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType   = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue     = { -10.f, -10.f, -10.f };
  settings.maxValue     = { 10.f, 10.f, 10.f };
  settings.defaultValue = { 0.f, 1.f, 0.f };
  settings.scaleLocked  = false;

  assignField(new ConstEmitterFieldEditor(), settings);
}

EmitterSpawnVelocityEditor::~EmitterSpawnVelocityEditor() {}

bool EmitterSpawnVelocityEditor::updateEmitterSpecs(
  ParticleEmitter::Specs& specs) const
{
  bool changed = false;

  changed =
    m_field->updateEmitterField(specs.spawnVelocity->getVelocityFieldPtr())
    || changed;

  return changed;
}

EmitterSpawnScaleEditor::EmitterSpawnScaleEditor()
  : EmitterAttributeEditor("Spawn scale")
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType   = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue     = { 0.1f, 0.1f, 0.1f };
  settings.maxValue     = { 5.f, 5.f, 5.f };
  settings.defaultValue = { 1.f, 1.f, 1.f };
  settings.scaleLocked  = true;

  assignField(new ConstEmitterFieldEditor(), settings);
}

EmitterSpawnScaleEditor::~EmitterSpawnScaleEditor() {}

bool EmitterSpawnScaleEditor::updateEmitterSpecs(
  ParticleEmitter::Specs& specs) const
{
  bool changed = false;

  changed = m_field->updateEmitterField(specs.spawnScale->getScaleFieldPtr())
         || changed;

  return changed;
}

EmitterSpawnColorEditor::EmitterSpawnColorEditor()
  : EmitterAttributeEditor("Spawn color")
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType   = EmitterScalarEditor::EScalarType::eColor4;
  settings.minValue     = { 0.f, 0.f, 0.f, 0.f };
  settings.maxValue     = { 1.f, 1.f, 1.f, 1.f };
  settings.defaultValue = { 1.f, 0.f, 0.f, 1.f };
  settings.scaleLocked  = false;

  assignField(new ConstEmitterFieldEditor(), settings);
}

EmitterSpawnColorEditor::~EmitterSpawnColorEditor() {}

bool EmitterSpawnColorEditor::updateEmitterSpecs(
  ParticleEmitter::Specs& specs) const
{
  bool changed = false;

  changed = m_field->updateEmitterField(specs.spawnColor->getColorFieldPtr())
         || changed;

  return changed;
}
