#pragma once

#include <string>
#include <glm/vec2.hpp>

namespace vrm
{
  
  struct OSFileDrop
  {
    glm::ivec2 cursorPos;
    const char* files = nullptr; // Separated with ;

    void clear()
    {
      cursorPos = { 0, 0 };
      files = nullptr;
    }
  };

} // namespace vrm
