#pragma once

#include "Vroom/Render/ParticleEmitter.h"
#include "Vroom/Render/ParticleEmitterAttribute.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"

namespace vrm::editor
{

  class EmitterAttributeEditor : public ImGuiElement
  {
  public:

    struct Specs
    {
      EmitterFieldEditor::EType type = EmitterFieldEditor::EType::eScalar;
      float min = -10.f, max = 10.f;
      bool scaleLocked = false;
    };

  public:

    EmitterAttributeEditor(const std::string& displayName, ParticleEmitter::Specs::EAttributeName name);
    ~EmitterAttributeEditor();

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;
    
    void setValue(const ParticleEmitterAttributeBase& emitterAttribute);

    void setName(const std::string& name);
    void setEmitterSpecsOffset(size_t offset);
    void setSpecs(const Specs& specs);
  
  protected:

    void onImgui() override;

  private:

    ParticleEmitter::Specs::EAttributeName m_name;
    std::string m_displayName;

    ConstParticleField m_spawnValue;
    ConstParticleField m_deathValue;

  };



}