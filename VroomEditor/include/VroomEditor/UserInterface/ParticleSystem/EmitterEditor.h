#pragma once

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterAttributeEditor.h"
#include <vector>


namespace vrm::editor
{

  class EmitterEditor : public ImGuiElement
  {
  public:

    EmitterEditor(const ParticleEmitter::Specs& initSpecs);
    ~EmitterEditor();

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;

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

  private:

    std::vector<EmitterAttributeEditor> m_attributes;


  };

}