#pragma once

#include "Vroom/Render/ParticleEmitter.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include <memory>

namespace vrm::editor
{

  class EmitterAttributeEditor : public ImGuiElement
  {
  public:

    EmitterAttributeEditor(const std::string& displayName, ParticleEmitter::Specs::EAttributeName name);
    ~EmitterAttributeEditor();

    EmitterAttributeEditor& operator=(const EmitterAttributeEditor& other) = delete;
    EmitterAttributeEditor(const EmitterAttributeEditor& other) = delete;

    EmitterAttributeEditor& operator=(EmitterAttributeEditor&& other) = default;
    EmitterAttributeEditor(EmitterAttributeEditor&& other) = default;

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;
    
    void setValueFromSpecs(const ParticleEmitter::Specs& specs);
  
  protected:

    void onImgui() override;

  protected:

    std::unique_ptr<EmitterFieldEditor> m_spawnField;
    std::unique_ptr<EmitterFieldEditor> m_deathField;

  private:

    ParticleEmitter::Specs::EAttributeName m_name;
    std::string m_displayName;

  };

  class EmitterPositionEditor : public EmitterAttributeEditor
  {
  public:

    EmitterPositionEditor();
    ~EmitterPositionEditor();

    EmitterPositionEditor& operator=(const EmitterPositionEditor& other) = delete;
    EmitterPositionEditor(const EmitterPositionEditor& other) = delete;

    EmitterPositionEditor& operator=(EmitterPositionEditor&& other) = default;
    EmitterPositionEditor(EmitterPositionEditor&& other) = default;

  protected:



  private:

  };

  class EmitterScaleEditor : public EmitterAttributeEditor
  {
  public:

    EmitterScaleEditor();
    ~EmitterScaleEditor();

    EmitterScaleEditor& operator=(const EmitterScaleEditor& other) = delete;
    EmitterScaleEditor(const EmitterScaleEditor& other) = delete;

    EmitterScaleEditor& operator=(EmitterScaleEditor&& other) = default;
    EmitterScaleEditor(EmitterScaleEditor&& other) = default;

  protected:

  private:

  };

  class EmitterColorEditor : public EmitterAttributeEditor
  {
  public:

    EmitterColorEditor();
    ~EmitterColorEditor();

    EmitterColorEditor& operator=(const EmitterColorEditor& other) = delete;
    EmitterColorEditor(const EmitterColorEditor& other) = delete;

    EmitterColorEditor& operator=(EmitterColorEditor&& other) = delete;
    EmitterColorEditor(EmitterColorEditor&& other) = delete;

  protected:

  private:

  };



}