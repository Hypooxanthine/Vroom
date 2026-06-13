#pragma once

#include <functional>
#include <list>
#include <optional>
#include <utility>
#include <vector>

#include "Rasterizer/Shader.h"


namespace vrm::render
{

class ShaderManager
{
public:

  class Handle
  {
  public:

    friend ShaderManager;

  private:

    Handle() = default;

  private:

    uint32_t m_key        = -1;
    uint32_t m_generation = -1;
  };

public:

  ShaderManager();

  Handle      emplace(gl::Shader&& shader);
  bool        update(Handle handle, gl::Shader&& shader);

  template <typename Func, typename... Args>
  void withShader(Handle handle, Func&& func, Args&&... args);

  template <typename Func, typename... Args>
  void withShader(Handle handle, Func&& func, Args&&... args) const;

private:

  struct _Slot
  {
    uint32_t                  generation = -1;
    std::optional<gl::Shader> shader;
  };

private:

  Handle _getValidHandle();
  _Slot* _tryGet(Handle handle);
  const _Slot* _tryGet(Handle handle) const;

private:

  std::vector<_Slot> m_slots;
  std::list<size_t>  m_availableSlotIndice;
};

template <typename Func, typename... Args>
void ShaderManager::withShader(Handle handle, Func&& func, Args&&... args)
{
  _Slot* slot = _tryGet(handle);

  if (slot)
  {
    assert(slot->shader.has_value());
    std::invoke(func, slot->shader.value(), std::forward<Args>(args)...);
  }
}

template <typename Func, typename... Args>
void ShaderManager::withShader(Handle handle, Func&& func, Args&&... args) const
{
  const _Slot* slot = _tryGet(handle);

  if (slot)
  {
    assert(slot->shader.has_value());
    std::invoke(func, slot->shader.value(), std::forward<Args>(args)...);
  }
}

} // namespace vrm::render
