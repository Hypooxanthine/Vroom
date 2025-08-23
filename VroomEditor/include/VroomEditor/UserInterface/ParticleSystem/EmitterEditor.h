#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include <string>
#include <vector>


namespace vrm::editor
{

  class EmitterEditor : public ImGuiElement
  {
  public:

    EmitterEditor(const ParticleEmitter::Specs& initSpecs);
    ~EmitterEditor();

    bool requestedDelete() const { return m_requestDelete; }

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;
    void setName(const std::string& name);

  protected:

    void onImgui() override;
  
  private:

    void _initAttributes(const ParticleEmitter::Specs& initSpecs);

    void _addAttribute(
      const ParticleEmitter::Specs& initSpecs,
      const EmitterAttributeEditor::Settings& settings,
      const std::string& displayName,
      ParticleEmitter::Specs::EAttributeName name
    );

    void _showContextualMenu();

  private:

    mutable bool m_changed = false;
    bool m_requestDelete = false;
    float m_emitRate;
    float m_lifeTime;
    std::string m_name = "Attribute";
    std::vector<EmitterAttributeEditor> m_attributes;

  };

}