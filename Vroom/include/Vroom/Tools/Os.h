#pragma once

#include "Vroom/Api.h"
#include <string>

namespace vrm
{
  
  class VRM_API OS
  {
  public:

    static void Run(const std::string& command, bool detach);

    static void RunInShell(const std::string& script, bool detach);

  };

} // namespace vrm::os
