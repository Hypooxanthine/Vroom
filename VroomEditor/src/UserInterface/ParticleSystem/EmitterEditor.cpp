#include "VroomEditor/UserInterface/ParticleSystem/EmitterEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include "imgui.h"

using namespace vrm::editor;

EmitterEditor::EmitterEditor()
{
  m_attributes.emplace_back(new EmitterPositionEditor());
  m_attributes.emplace_back(new EmitterScaleEditor());
  m_attributes.emplace_back(new EmitterColorEditor());
}

EmitterEditor::~EmitterEditor()
{

}

void EmitterEditor::setName(const std::string& name)
{
  m_name = name;
}

bool EmitterEditor::updateEmitterSpecs(ParticleEmitter::Specs& specs) const
{
  bool changed = m_changed;

  if (m_changed)
  {
    specs.emitRate = m_emitRate;
    specs.lifeTime.getRawData()[0] = m_lifeTime;
    
    m_changed = false;
  }

  for (const std::unique_ptr<EmitterAttributeEditor>& attribute : m_attributes)
  {
    changed = attribute->updateEmitterSpecs(specs) || changed;
  }

  return changed;
}

void EmitterEditor::onImgui()
{
  bool open = ImGui::TreeNodeEx(m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);

  if (ImGui::BeginPopupContextItem("Contextual menu"))
  {
    _showContextualMenu();
    ImGui::EndPopup();
  }

  if (open)
  {
    ImGui::TreePush("Life cycle");
    {
      m_changed = ImGui::SliderFloat("Emit rate", &m_emitRate, 0.1f, 500.f, "%.1f", ImGuiSliderFlags_Logarithmic) || m_changed;
      m_changed = ImGui::SliderFloat("Life time", &m_lifeTime, 0.1f, 20.f) || m_changed;

      ImGui::TreePop();
    }

    for (std::unique_ptr<EmitterAttributeEditor>& attribute : m_attributes)
    {
      attribute->renderImgui();
    }

    ImGui::TreePop();
  }
}

void EmitterEditor::onUpdate(const DeltaTime& dt)
{
  for (auto& attributeEditor : m_attributes)
  {
    attributeEditor->update(dt);
  }
}

void EmitterEditor::_showContextualMenu()
{
  m_requestDelete = ImGui::Selectable("Delete");
}
