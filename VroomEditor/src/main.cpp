#include <Vroom/Core/Application.h>

#include "VroomEditor/EditorLayer.h"

#include "imgui.h"

using namespace vrm;

int main(int argc, char** argv)
{
    {
        Application app(argc, argv);

        app.pushLayer<EditorLayer>();

        app.run();
    }
    return 0;
}