#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include <cstring>

#include "imgui.h"

#include "Vroom/Render/ParticleEmitterField.h"

using namespace vrm::editor;

EmitterFieldEditor::EmitterFieldEditor(EType::Type type)
  : m_type(type), m_nextType(type)
{}

EmitterFieldEditor::~EmitterFieldEditor() {}

void EmitterFieldEditor::setName(const std::string& name) { m_name = name; }

bool EmitterFieldEditor::updateEmitterField(
  ParticleEmitterFieldType& field) const
{
  return onUpdateEmitterField(field);
}

void EmitterFieldEditor::onImgui()
{
  bool open =
    ImGui::TreeNodeEx(getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

  if (ImGui::BeginPopupContextItem("Contextual menu"))
  {
    _showContextualMenu();
    ImGui::EndPopup();
  }

  if (open)
  {
    onImguiEdit();
    ImGui::TreePop();
  }
}

void EmitterFieldEditor::_showContextualMenu()
{
  for (size_t i = 0; i < EType::eCount; ++i)
  {
    EType::Type type       = EType::Type(i);
    std::string str        = std::to_string(type);
    bool        isThisType = m_type == type;

    if (isThisType) ImGui::BeginDisabled();

    if (ImGui::Selectable(str.c_str())) { m_nextType = type; }

    if (isThisType) ImGui::EndDisabled();
  }
}

/* CONST PARTICLE FIELD EDITOR */

ConstEmitterFieldEditor::ConstEmitterFieldEditor()
  : EmitterFieldEditor(EType::eConstEmitterField)
{}

ConstEmitterFieldEditor::~ConstEmitterFieldEditor() {}

void ConstEmitterFieldEditor::onImguiEdit() { m_scalarEditor.renderImgui(); }

bool ConstEmitterFieldEditor::onUpdateEmitterField(
  ParticleEmitterFieldType& field) const
{
  ConstParticleEmitterField newField;

  if (m_scalarEditor.getModified())
  {
    std::span<float const> editorData = m_scalarEditor.getData();
    std::memcpy(&newField.value.x, editorData.data(), editorData.size_bytes());
    field = newField;

    return true;
  }

  return false;
}

/* RANDOM RANGE PARTICLE FIELD EDITOR */

RandomRangeEmitterFieldEditor::RandomRangeEmitterFieldEditor()
  : EmitterFieldEditor(EType::eRandomRangeEmitterField)
{}

RandomRangeEmitterFieldEditor::~RandomRangeEmitterFieldEditor() {}

void RandomRangeEmitterFieldEditor::onImguiEdit()
{
  ImGui::PushID("MinRange");
  ImGui::Text("%s:", "Min range");
  ImGui::SameLine();
  m_minRange.renderImgui();
  ImGui::PopID();

  ImGui::PushID("MaxRange");
  ImGui::Text("%s:", "Max range");
  ImGui::SameLine();
  m_maxRange.renderImgui();
  ImGui::PopID();
}

bool RandomRangeEmitterFieldEditor::onUpdateEmitterField(
  ParticleEmitterFieldType& field) const
{
  bool changed = false;

  RandomRangeEmitterField newField;

  if (m_minRange.getModified())
  {
    std::span<float const> editorData = m_minRange.getData();
    std::memcpy(&newField.minValue.x, editorData.data(),
                editorData.size_bytes());

    changed = true;
  }

  if (m_maxRange.getModified())
  {
    std::span<float const> editorData = m_maxRange.getData();
    std::memcpy(&newField.maxValue.x, editorData.data(),
                editorData.size_bytes());

    changed = true;
  }

  if (changed) { field = newField; }

  return changed;
}
