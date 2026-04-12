#include "Renderer/CustomPassSlot.h"

using namespace vrm;

void CustomPassSlot::clear()
{
  m_factories.clear();
}

void CustomPassSlot::pushPass(std::unique_ptr<RenderPassFactory>&& factory)
{
  m_factories.emplace_back(std::move(factory));
}

CustomPassSlot::iterator CustomPassSlot::begin()
{
  return m_factories.begin();
}

CustomPassSlot::const_iterator CustomPassSlot::begin() const
{
  return m_factories.begin();
}

CustomPassSlot::const_iterator CustomPassSlot::cbegin() const
{
  return m_factories.cbegin();
}

CustomPassSlot::iterator CustomPassSlot::end()
{
  return m_factories.end();
}

CustomPassSlot::const_iterator CustomPassSlot::end() const
{
  return m_factories.end();
}

CustomPassSlot::const_iterator CustomPassSlot::cend() const
{
  return m_factories.cend();
}
