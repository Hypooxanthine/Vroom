#pragma once

#include "Vroom/Api.h"
#include <string>

namespace vrm
{
  
  class VRM_API OS
  {
  public:

    static void RunInShell(const std::string& command, bool detach);

  };

} // namespace vrm::os
