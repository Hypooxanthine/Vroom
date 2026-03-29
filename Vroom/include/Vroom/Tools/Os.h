#pragma once

#include "Vroom/Api.h"
#include <future>
#include <string>

namespace vrm
{
  
  class VRM_API OS
  {
  public:

    [[nodiscard]] static std::future<int> Run(const std::string& command);

    [[nodiscard]] static std::future<int> RunInShell(const std::string& script);

  };

} // namespace vrm::os
