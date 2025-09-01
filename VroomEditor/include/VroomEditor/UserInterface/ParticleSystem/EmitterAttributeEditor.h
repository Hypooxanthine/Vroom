#pragma once

#include "Vroom/Core/DeltaTime.h"
#include "Vroom/Render/ParticleEmitter.h"
#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"
#include <memory>

namespace vrm::editor
{

  class EmitterAttributeEditor : public ImGuiElement
  {
  public:

    EmitterAttributeEditor(const std::string& displayName, ParticleEmitterAttributeBase::EAttributeName name);
    ~EmitterAttributeEditor();

    EmitterAttributeEditor& operator=(const EmitterAttributeEditor& other) = delete;
    EmitterAttributeEditor(const EmitterAttributeEditor& other) = delete;

    EmitterAttributeEditor& operator=(EmitterAttributeEditor&& other) = default;
    EmitterAttributeEditor(EmitterAttributeEditor&& other) = default;

    bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const;
  
  protected:

    void onImgui() override;
    void onUpdate(const DeltaTime& dt) override;

    void assignField(std::unique_ptr<EmitterFieldEditor>& field, EmitterFieldEditor* newField, const EmitterScalarEditor::Settings& settings);
    inline void assignSpawnField(EmitterFieldEditor* emitterFieldEditor)
    {
      assignField(m_spawnField, emitterFieldEditor, m_spawnFieldSettings);
      m_spawnField->setName("Spawn");
    }

    inline void assignDeathField(EmitterFieldEditor* emitterFieldEditor)
    {
      assignField(m_deathField, emitterFieldEditor, m_deathFieldSettings);
      m_deathField->setName("Death");
    }

  protected:

    std::unique_ptr<EmitterFieldEditor> m_spawnField;
    std::unique_ptr<EmitterFieldEditor> m_deathField;
    EmitterScalarEditor::Settings m_spawnFieldSettings, m_deathFieldSettings;

  private:

    ParticleEmitterAttributeBase::EAttributeName m_name;
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