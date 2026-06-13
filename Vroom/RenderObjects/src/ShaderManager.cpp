#include "RenderObjects/ShaderManager.h"
#include <cstdint>

using namespace vrm;
using namespace vrm::render;

ShaderManager::ShaderManager()
{}

ShaderManager::Handle ShaderManager::emplace(gl::Shader&& shader)
{
  Handle newHandle                   = _getValidHandle();
  m_slots.at(newHandle.m_key).shader = std::move(shader);

  return newHandle;
}

bool ShaderManager::update(Handle handle, gl::Shader&& shader)
{
  _Slot* slot = _tryGet(handle);

  if (slot == nullptr)
    return false;

  slot->shader = std::move(shader);
  return true;
}

ShaderManager::Handle ShaderManager::_getValidHandle()
{
  Handle handle;

  if (m_availableSlotIndice.empty())
  {
    handle.m_key        = static_cast<uint32_t>(m_slots.size());
    handle.m_generation = 0;

    _Slot& newSlot     = m_slots.emplace_back();
    newSlot.generation = handle.m_generation;
  }
  else
  {
    size_t index = m_availableSlotIndice.front();
    m_availableSlotIndice.pop_front();

    _Slot& slot = m_slots.at(index);
    ++slot.generation;
    handle.m_generation = slot.generation;
    handle.m_key        = index;
  }

  return handle;
}

ShaderManager::_Slot* ShaderManager::_tryGet(Handle handle)
{
  _Slot* slot = nullptr;

  if (handle.m_key < m_slots.size() && m_slots.at(handle.m_key).generation == handle.m_generation)
  {
    slot = &m_slots.at(handle.m_key);
  }

  return slot;
}

const ShaderManager::_Slot* ShaderManager::_tryGet(Handle handle) const
{
  const _Slot* slot = nullptr;

  if (handle.m_key < m_slots.size() && m_slots.at(handle.m_key).generation == handle.m_generation)
  {
    slot = &m_slots.at(handle.m_key);
  }

  return slot;
}
