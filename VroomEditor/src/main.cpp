#include <Vroom/Core/Application.h>

#include "VroomEditor/EditorLayer.h"
#include "VroomEditor/UserInterface/UserInterfaceLayer.h"

using namespace vrm;

int main(int argc, char **argv)
{
  {
    Application app(argc, argv);

    app.pushLayer<EditorLayer>();
    app.pushLayer<UserInterfaceLayer>();

    app.run();
  }
  return 0;
}