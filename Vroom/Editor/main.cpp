#include <Application/Application.h>

#include "Editor/EditorLayer.h"
#include "Editor/UserInterfaceLayer.h"

using namespace vrm;

int main(int argc, char** argv)
{
  {
    Application app(argc, argv);

    app.pushLayer<EditorLayer>();
    app.pushLayer<UserInterfaceLayer>();

    app.run();
  }
  return 0;
}
