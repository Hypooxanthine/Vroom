#ifdef _WIN32

#include <thread>

#include "Vroom/Tools/Os.h"

using namespace vrm;

void OS::RunInShell(const std::string& command, bool detach)
{
  std::string shellCmd;

  shellCmd = "start cmd /k \"" + command + "\"";

  auto t = std::thread([cmd = std::move(shellCmd)]() {
    system(cmd.c_str());
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


#endif // __unix__
