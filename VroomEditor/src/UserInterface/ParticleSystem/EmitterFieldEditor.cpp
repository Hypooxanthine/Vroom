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
  ImGui::Text("%s:", m_name.c_str());
  ImGui::SameLine();
  onImguiEdit();
}

/* CONST PARTICLE FIELD EDITOR */

void ConstEmitterFieldEditor::onImguiEdit()
{
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
  
}

void RandomRangeEmitterFieldEditor::onSetData(const ParticleEmitterFieldBase& field)
{
  
}

bool RandomRangeEmitterFieldEditor::onUpdateEmitterField(ParticleEmitterFieldBase& field) const
{
  return false;
}
