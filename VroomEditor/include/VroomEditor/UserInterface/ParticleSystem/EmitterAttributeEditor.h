#pragma once

#include <memory>

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterFieldEditor.h"
#include "VroomEditor/UserInterface/ParticleSystem/EmitterScalarEditor.h"

#include "Vroom/Core/DeltaTime.h"
#include "Vroom/Render/ParticleEmitter.h"

namespace vrm::editor
{

class EmitterAttributeEditor : public ImGuiElement
{
public:

  EmitterAttributeEditor(const std::string& displayName);
  ~EmitterAttributeEditor();

  EmitterAttributeEditor&
  operator=(const EmitterAttributeEditor& other)              = delete;
  EmitterAttributeEditor(const EmitterAttributeEditor& other) = delete;

  EmitterAttributeEditor& operator=(EmitterAttributeEditor&& other) = default;
  EmitterAttributeEditor(EmitterAttributeEditor&& other)            = default;

  virtual bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const = 0;

protected:

  void onImgui() override;
  void onUpdate(const DeltaTime& dt) override;

  void        assignField(std::unique_ptr<EmitterFieldEditor>& field,
                          EmitterFieldEditor*                  newField,
                          const EmitterScalarEditor::Settings& settings);
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

  std::unique_ptr<EmitterFieldEditor> m_spawnField;
  std::unique_ptr<EmitterFieldEditor> m_deathField;

protected:

  EmitterScalarEditor::Settings m_spawnFieldSettings, m_deathFieldSettings;

private:

  std::string m_displayName;
};

class EmitterPositionEditor : public EmitterAttributeEditor
{
public:

  EmitterPositionEditor();
  ~EmitterPositionEditor();

  EmitterPositionEditor& operator=(const EmitterPositionEditor& other) = delete;
  EmitterPositionEditor(const EmitterPositionEditor& other)            = delete;

  EmitterPositionEditor& operator=(EmitterPositionEditor&& other) = default;
  EmitterPositionEditor(EmitterPositionEditor&& other)            = default;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

class EmitterScaleEditor : public EmitterAttributeEditor
{
public:

  EmitterScaleEditor();
  ~EmitterScaleEditor();

  EmitterScaleEditor& operator=(const EmitterScaleEditor& other) = delete;
  EmitterScaleEditor(const EmitterScaleEditor& other)            = delete;

  EmitterScaleEditor& operator=(EmitterScaleEditor&& other) = default;
  EmitterScaleEditor(EmitterScaleEditor&& other)            = default;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

class EmitterColorEditor : public EmitterAttributeEditor
{
public:

  EmitterColorEditor();
  ~EmitterColorEditor();

  EmitterColorEditor& operator=(const EmitterColorEditor& other) = delete;
  EmitterColorEditor(const EmitterColorEditor& other)            = delete;

  EmitterColorEditor& operator=(EmitterColorEditor&& other) = delete;
  EmitterColorEditor(EmitterColorEditor&& other)            = delete;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

} // namespace vrm::editor
