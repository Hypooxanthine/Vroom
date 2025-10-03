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

  void assignField(EmitterFieldEditor*                  newField,
                   const EmitterScalarEditor::Settings& settings);

  std::unique_ptr<EmitterFieldEditor> m_field;

private:

  std::string                   m_displayName;
  EmitterScalarEditor::Settings m_scalarSettings;
};

class EmitterSpawnPositionEditor : public EmitterAttributeEditor
{
public:

  EmitterSpawnPositionEditor();
  ~EmitterSpawnPositionEditor();

  EmitterSpawnPositionEditor&
  operator=(const EmitterSpawnPositionEditor& other)                  = delete;
  EmitterSpawnPositionEditor(const EmitterSpawnPositionEditor& other) = delete;

  EmitterSpawnPositionEditor&
  operator=(EmitterSpawnPositionEditor&& other)                  = default;
  EmitterSpawnPositionEditor(EmitterSpawnPositionEditor&& other) = default;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

class EmitterSpawnVelocityEditor : public EmitterAttributeEditor
{
public:

  EmitterSpawnVelocityEditor();
  ~EmitterSpawnVelocityEditor();

  EmitterSpawnVelocityEditor&
  operator=(const EmitterSpawnVelocityEditor& other)                  = delete;
  EmitterSpawnVelocityEditor(const EmitterSpawnVelocityEditor& other) = delete;

  EmitterSpawnVelocityEditor&
  operator=(EmitterSpawnVelocityEditor&& other)                  = default;
  EmitterSpawnVelocityEditor(EmitterSpawnVelocityEditor&& other) = default;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

class EmitterSpawnScaleEditor : public EmitterAttributeEditor
{
public:

  EmitterSpawnScaleEditor();
  ~EmitterSpawnScaleEditor();

  EmitterSpawnScaleEditor&
  operator=(const EmitterSpawnScaleEditor& other)               = delete;
  EmitterSpawnScaleEditor(const EmitterSpawnScaleEditor& other) = delete;

  EmitterSpawnScaleEditor& operator=(EmitterSpawnScaleEditor&& other) = default;
  EmitterSpawnScaleEditor(EmitterSpawnScaleEditor&& other)            = default;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

class EmitterSpawnColorEditor : public EmitterAttributeEditor
{
public:

  EmitterSpawnColorEditor();
  ~EmitterSpawnColorEditor();

  EmitterSpawnColorEditor&
  operator=(const EmitterSpawnColorEditor& other)               = delete;
  EmitterSpawnColorEditor(const EmitterSpawnColorEditor& other) = delete;

  EmitterSpawnColorEditor& operator=(EmitterSpawnColorEditor&& other) = delete;
  EmitterSpawnColorEditor(EmitterSpawnColorEditor&& other)            = delete;

protected:

  bool updateEmitterSpecs(ParticleEmitter::Specs& specs) const override;

private:
};

} // namespace vrm::editor
