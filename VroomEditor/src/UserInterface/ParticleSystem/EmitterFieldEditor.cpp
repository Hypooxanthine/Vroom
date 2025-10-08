#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include <algorithm>
#include <cstring>

#include "imgui.h"

#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/ParticleEmitterField.h"

using namespace vrm::editor;

EmitterFieldEditor::EmitterFieldEditor(EType::Type type)
  : m_type(type), m_nextType(type)
{}

EmitterFieldEditor::~EmitterFieldEditor() {}

void EmitterFieldEditor::setName(const std::string& name) { m_name = name; }

bool EmitterFieldEditor::updateEmitterField(
  std::unique_ptr<IEmitterField>& field) const
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
    EType::Type type = EType::Type(i);

    if (!m_supportedTypes.contains(type)) continue;

    std::string str        = std::to_string(type);
    bool        isThisType = m_type == type;

    if (isThisType) ImGui::BeginDisabled();

    if (ImGui::Selectable(str.c_str()))
    {
      m_nextType = type;
    }

    if (isThisType) ImGui::EndDisabled();
  }
}

/* CONST PARTICLE FIELD EDITOR */

ConstEmitterFieldEditor::ConstEmitterFieldEditor()
  : EmitterFieldEditor(EType::eConst)
{}

ConstEmitterFieldEditor::~ConstEmitterFieldEditor() {}

void ConstEmitterFieldEditor::onImguiEdit() { m_scalarEditor.renderImgui(); }

bool ConstEmitterFieldEditor::onUpdateEmitterField(
  std::unique_ptr<IEmitterField>& field) const
{
  if (m_scalarEditor.getAndResetModified())
  {
    if (field->getType() != EmitterFieldType::eConst)
    {
      switch (m_scalarEditor.getDimension())
      {
      case 1:
        field.reset(new ConstEmitterField1);
        break;
      case 2:
        field.reset(new ConstEmitterField2);
        break;
      case 3:
        field.reset(new ConstEmitterField3);
        break;
      case 4:
        field.reset(new ConstEmitterField4);
        break;
      default:
        VRM_ASSERT_MSG(false, "Dimension must be from 1 to 4");
      }
    }

    IConstEmitterField& asConst = static_cast<IConstEmitterField&>(*field);

    std::span<float const> editorData = m_scalarEditor.getData();
    asConst.setValue(editorData);

    return true;
  }
  else return false;
}

/* RANDOM RANGE PARTICLE FIELD EDITOR */

RandomRangeEmitterFieldEditor::RandomRangeEmitterFieldEditor()
  : EmitterFieldEditor(EType::eRandomRange)
{}

RandomRangeEmitterFieldEditor::~RandomRangeEmitterFieldEditor() {}

void RandomRangeEmitterFieldEditor::onImguiEdit()
{
  ImGui::PushID("MinRange");
  ImGui::Text("%s:", "Min");
  ImGui::SameLine();
  m_minRange.renderImgui();
  ImGui::PopID();

  ImGui::PushID("MaxRange");
  ImGui::Text("%s:", "Max");
  ImGui::SameLine();
  m_maxRange.renderImgui();
  ImGui::PopID();
}

bool RandomRangeEmitterFieldEditor::onUpdateEmitterField(
  std::unique_ptr<IEmitterField>& field) const
{
  if (m_minRange.getAndResetModified() || m_maxRange.getAndResetModified())
  {
    if (field->getType() != EmitterFieldType::eRandomRange)
    {
      switch (m_minRange.getDimension())
      {
      case 1:
        field.reset(new RandomRangeEmitterField1);
        break;
      case 2:
        field.reset(new RandomRangeEmitterField2);
        break;
      case 3:
        field.reset(new RandomRangeEmitterField3);
        break;
      case 4:
        field.reset(new RandomRangeEmitterField4);
        break;
      default:
        VRM_ASSERT_MSG(false, "Dimension must be from 1 to 4");
      }
    }

    IRandomRangeEmitterField& asRandomRange =
      static_cast<IRandomRangeEmitterField&>(*field);

    std::span<float const> editorMinData = m_minRange.getData();
    std::span<float const> editorMaxData = m_maxRange.getData();
    asRandomRange.setValue(editorMinData, editorMaxData);

    return true;
  }
  else return false;
}

/* RANDOM RANGE PARTICLE FIELD EDITOR */

RandomConeEmitterFieldEditor::RandomConeEmitterFieldEditor()
  : EmitterFieldEditor(EType::eRandomCone)
{
  EmitterScalarEditor::Settings settings;
  settings.scalarType   = EmitterScalarEditor::EScalarType::eScalar;
  settings.defaultValue = { 0.f };
  settings.minValue     = { 0.f };
  settings.scaleLocked  = false;

  settings.maxValue = { 180.f }; // 180 degrees
  m_angle.setSettings(settings);

  settings.maxValue     = { 100.f };
  settings.defaultValue = { 1.f };
  m_length.setSettings(settings);

  settings.scalarType   = EmitterScalarEditor::EScalarType::eVec3;
  settings.minValue     = { -1.f, -1.f, -1.f };
  settings.maxValue     = { 1.f, 1.f, 1.f };
  settings.defaultValue = { 0.f, 1.f, 0.f };
  m_direction.setSettings(settings);
}

RandomConeEmitterFieldEditor::~RandomConeEmitterFieldEditor() {}

void RandomConeEmitterFieldEditor::onImguiEdit()
{
  ImGui::PushID("Direction");
  ImGui::Text("%s:", "Direction");
  ImGui::SameLine();
  m_direction.renderImgui();
  ImGui::PopID();

  ImGui::PushID("Angle");
  ImGui::Text("%s:", "Angle");
  ImGui::SameLine();
  m_angle.renderImgui();
  ImGui::PopID();

  ImGui::PushID("Length");
  ImGui::Text("%s:", "Length");
  ImGui::SameLine();
  m_length.renderImgui();
  ImGui::PopID();
}

bool RandomConeEmitterFieldEditor::onUpdateEmitterField(
  std::unique_ptr<IEmitterField>& field) const
{
  if (m_direction.getAndResetModified() || m_angle.getAndResetModified()
      || m_length.getAndResetModified())
  {
    if (field->getType() != EmitterFieldType::eRandomCone)
    {
      field.reset(new RandomConeEmitterField());
    }

    RandomConeEmitterField& asRandomCone =
      static_cast<RandomConeEmitterField&>(*field);

    std::span<float const> editorDirectionData = m_direction.getData();
    glm::vec3              direction;
    std::copy_n(editorDirectionData.begin(),
                std::min<size_t>(editorDirectionData.size(), 3), &direction.x);
    asRandomCone.setDirection(direction);

    std::span<float const> editorAngleData = m_angle.getData();
    asRandomCone.setAngle(glm::radians(editorAngleData[0]));

    std::span<float const> editorLengthData = m_length.getData();
    asRandomCone.setLength(editorLengthData[0]);

    return true;
  }
  else return false;
}
