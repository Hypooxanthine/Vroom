#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include <cstring>

#include "imgui.h"

#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/ParticleEmitterField.h"
#include "glm/ext/scalar_constants.hpp"

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
    if (field->getType() != EmitterFieldType::Const)
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
    if (field->getType() != EmitterFieldType::RandomRange)
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

  settings.maxValue = { glm::pi<float>() };
  m_angle.setSettings(settings);

  settings.maxValue = { 100.f };
  m_length.setSettings(settings);
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
  //
  return false;
}

void RandomConeEmitterFieldEditor::onUpdateScalarSettings(
  const EmitterScalarEditor::Settings& settings)
{
  VRM_ASSERT_MSG(settings.scalarType == EmitterScalarEditor::EScalarType::eVec3,
                 "Unsupported scalar type");

  m_direction.setSettings(settings);
}
