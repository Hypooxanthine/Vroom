#include <thread>

#include "Vroom/Tools/Os.h"

using namespace vrm;

void OS::Run(const std::string& command, bool detach)
{
  auto t = std::thread([command]() {
    system(command.c_str());
  });

  if (detach)
  {
    t.detach();
  }
  else
  {
    t.join();
  }
}