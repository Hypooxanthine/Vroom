#ifdef __unix__

#include <thread>

#include "Vroom/Tools/Os.h"

using namespace vrm;

std::future<int> OS::RunInShell(const std::string& script, bool detach)
{
  std::string shellCmd = "gnome-terminal -- bash -c \"" + script + "; exec bash\"";

  return std::async([](const std::string& command) {
    return system(command.c_str());
  }, shellCmd);
}

#endif // __unix__
