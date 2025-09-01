#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "Vroom/Core/Assert.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterFieldEditor::EmitterFieldEditor(EType::Type type)
  : m_type(type), m_nextType(type)
{

}

EmitterFieldEditor::~EmitterFieldEditor()
{

}

void EmitterFieldEditor::setName(const std::string& name)
{
  m_name = name;
}

bool EmitterFieldEditor::updateEmitterField(ParticleEmitterFieldBase& field) const
{
  return onUpdateEmitterField(field);
}

void EmitterFieldEditor::onImgui()
{
  bool open = ImGui::TreeNodeEx(getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen);

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
    EType::Type type = EType::Type(i);
    std::string str = std::to_string(type);
    bool isThisType = m_type == type;

    if (isThisType)
      ImGui::BeginDisabled();

    if (ImGui::Selectable(str.c_str()))
    {
      m_nextType = type;
    }

    if (isThisType)
      ImGui::EndDisabled();
  }
}

/* CONST PARTICLE FIELD EDITOR */

ConstEmitterFieldEditor::ConstEmitterFieldEditor()
 : EmitterFieldEditor(EType::eConstEmitterField)
{

}

ConstEmitterFieldEditor::~ConstEmitterFieldEditor()
{

}

void ConstEmitterFieldEditor::onImguiEdit()
{
  m_scalarEditor.renderImgui();
}

bool ConstEmitterFieldEditor::onUpdateEmitterField(ParticleEmitterFieldBase& field) const
{
  if (m_scalarEditor.getModified())
  {
    std::span<float> emitterFieldData = field.getRawData();
    std::span<float const> editorData = m_scalarEditor.getData();
    VRM_ASSERT_MSG(emitterFieldData.size() == editorData.size(), "Unexpected error: emitter field size mismatch");

    for (size_t i = 0; i < emitterFieldData.size(); ++i)
    {
      emitterFieldData[i] = editorData[i];
    }

    return true;
  }

  return false;
}

/* RANDOM RANGE PARTICLE FIELD EDITOR */

RandomRangeEmitterFieldEditor::RandomRangeEmitterFieldEditor()
 : EmitterFieldEditor(EType::eRandomRangeEmitterField)
{

}

RandomRangeEmitterFieldEditor::~RandomRangeEmitterFieldEditor()
{

}

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

bool RandomRangeEmitterFieldEditor::onUpdateEmitterField(ParticleEmitterFieldBase& field) const
{
  if (m_minRange.getModified())
  {
    std::span<float> emitterFieldData = field.getRawData();
    std::span<float const> editorData = m_minRange.getData();
    VRM_ASSERT_MSG(emitterFieldData.size() == editorData.size(), "Unexpected error: emitter field size mismatch");

    for (size_t i = 0; i < emitterFieldData.size(); ++i)
    {
      emitterFieldData[i] = editorData[i];
    }

    return true;
  }

  return false;
}
