#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "Vroom/Core/Assert.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterFieldEditor::EmitterFieldEditor()
{

}

EmitterFieldEditor::~EmitterFieldEditor()
{

}

void EmitterFieldEditor::setName(const std::string& name)
{
  m_name = name;
}

void EmitterFieldEditor::setData(const ParticleEmitterFieldBase& field)
{
  onSetData(field);
}

bool EmitterFieldEditor::updateEmitterField(ParticleEmitterFieldBase& field) const
{
  return onUpdateEmitterField(field);
}

void EmitterFieldEditor::onImgui()
{
  onImguiEdit();
}

/* CONST PARTICLE FIELD EDITOR */

void ConstEmitterFieldEditor::onImguiEdit()
{
  ImGui::Text("%s:", getName().c_str());
  ImGui::SameLine();
  m_scalarEditor.renderImgui();
}

void ConstEmitterFieldEditor::onSetData(const ParticleEmitterFieldBase& field)
{
  m_scalarEditor.setData(field.getRawData());
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

void RandomRangeEmitterFieldEditor::onImguiEdit()
{
  if (ImGui::TreeNode(getName().c_str()))
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

    ImGui::TreePop();
  }
}

void RandomRangeEmitterFieldEditor::onSetData(const ParticleEmitterFieldBase& field)
{
  m_minRange.setData(field.getRawData());
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
