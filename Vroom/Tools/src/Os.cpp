#include <future>

#include "Tools/Os.h"

using namespace vrm;

std::future<int> OS::Run(const std::string& command)
{
  return std::async(
    [](const std::string& command)
    {
      return system(command.c_str());
    },
    command);
}
