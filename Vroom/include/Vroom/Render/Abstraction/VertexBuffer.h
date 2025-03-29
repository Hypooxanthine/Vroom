#pragma once

#include "Vroom/Render/Abstraction/Buffer.h"

namespace vrm::gl
{

  template <typename T>
  class VertexBuffer : public ArrayBuffer<T, GL_ARRAY_BUFFER>
  {
  };

} // namespace vrm::gl
