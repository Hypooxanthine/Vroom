#pragma once

#include <string>
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "VroomEditor/UserInterface/ImGuiElement.h"

#include "Vroom/Core/PerfRecorder.h"
#include "Vroom/Core/Timer.h"

namespace vrm::editor
{

class ProfilingPanel : public ImGuiElement
{
public:

  ProfilingPanel();
  ~ProfilingPanel();

  ProfilingPanel& operator=(const ProfilingPanel& other) = delete;
  ProfilingPanel(const ProfilingPanel& other)            = delete;

  ProfilingPanel& operator=(ProfilingPanel&& other) = delete;
  ProfilingPanel(ProfilingPanel&& other)            = delete;

protected:

  void onImgui() override;

private:

  struct ProfileEntry
  {
    std::string name;
    size_t      uid;
    double      startSec;
    double      endSec;
    float       parentUse;
    int         depth;
  };

private:

  void _showRecorderTree(const PerfRecorder& recorder);

  bool _buildProfileEntriesCheck(const PerfRecorder& recorder);
  void _buildProfileEntriesRecursive(const PerfRecorder& recorder,
                                     uint64_t startNanosecs, int depth);

  ImU32 _lerpColor(ImU32 c1, ImU32 c2, float t);

private:

  std::vector<ProfileEntry> m_entries;
  int                       m_maxDepth = 0;
  Timer                     m_timer;

  std::string m_highlight;
};

} // namespace vrm::editor
