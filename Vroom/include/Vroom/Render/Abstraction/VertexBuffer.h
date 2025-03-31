#pragma once

#include <concepts>

#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/VertexAttribInfo.h"

namespace vrm::gl
{

  template <typename T>
  using VertexBuffer = ArrayBuffer<T, GL_ARRAY_BUFFER>;

} // namespace vrm::gl
