#ifdef _WIN32

#include "Tools/Os.h"

using namespace vrm;

std::future<int> OS::RunInShell(const std::string& script)
{
  std::string shellCmd;

  shellCmd = "start cmd /k \"" + script + "\"";

  return std::async([](const std::string& command) {
    return system(command.c_str());
  }, shellCmd);
}


#endif // __unix__
