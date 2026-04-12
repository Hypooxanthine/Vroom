#pragma once

#include <memory>
#include <vector>

#include "Renderer/RenderPassFactory.h"

namespace vrm
{

class CustomPassSlot
{
public:

  using container      = std::vector<std::unique_ptr<RenderPassFactory>>;
  using iterator       = container::iterator;
  using const_iterator = container::const_iterator;

public:

  CustomPassSlot()  = default;
  ~CustomPassSlot() = default;

  void clear();
  void pushPass(std::unique_ptr<RenderPassFactory>&& factory);

  iterator       begin();
  const_iterator begin() const;
  const_iterator cbegin() const;

  iterator       end();
  const_iterator end() const;
  const_iterator cend() const;

private:

  container m_factories;
};

} // namespace vrm
