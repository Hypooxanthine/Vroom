#pragma once

#include "Vroom/Render/Abstraction/StorageBuffer.h"

namespace vrm::gl
{

  class DynamicStorageBuffer : public StorageBuffer
  {
  public:
    inline constexpr DynamicStorageBuffer() : StorageBuffer()
    {
    }

    inline virtual ~DynamicStorageBuffer()
    {
    }

    inline void setData(const void* data, GLsizei dataSize, GLintptr offset = 0, GLenum usage = GL_DYNAMIC_DRAW)
    {
      if (getRenderID() == 0)
      {
        reset(dataSize + offset, usage);
      }

      /// @todo Ensure buffer has correct size

      // Assign data
      glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, dataSize, data);
    }

  private:
  };

} // namespace vrm::gl
