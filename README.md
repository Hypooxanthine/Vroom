
# Vroom

Vroom is a 3D game engine made with OpenGL. It is a personal project I'm doing to improve myself as a computer graphics and C++ developer.

## Features

- Rendering
  - MSAA
  - Directional lights shadow mapping
    - TODO: cascaded shadow maps
  - Kernel-based soft shadows
  - Clustered rendering for point lights
  - Modular shaders and materials
- Assets
  - Handful assets loader
  - Keeping track of (un)used asset for runtime memory management
- Inputs
  - Triggers, for callbacks on press/release-compatible events
  - Custom events, for callback on any events
- Scripting
  - Native C++ scripting
  - Reflexion on user scripts for scenes (de)serialization
- Editor
  - Resources navigation
  - Level opening/saving
  - Files DragAndDrop from OS explorer into the assets browser
  - Meshes, materials DragAndDrop on mesh components


## Screenshots

Editor
![Screenshot 1](Screenshots/SandboxScene.png)

Light complexity per cluster (clustered rendering)
![Screenshot 2](Screenshots/ClusteredCulling.png)

## Building project

```bash
git clone git@github.com:Hypooxanthine/Vroom.git --recursive
```

- In repository root folder, open a terminal and type:
```bash
mkdir build
cd build
cmake ..
ctest # If you want to run unit tests
doxygen ../Doxyfile # If you need to generate documentation
cmake --build . --target=VroomEditor --config=Release --parallel
```

```bash
cd VroomEditor # Don't use ./VroomEditor/VroomEditor directly because the program won't be able to load resource files.
./VroomEditor
```

## External libraries

- [glew](https://glew.sourceforge.net/)
- [glfw](https://www.glfw.org/)
- [glm](https://github.com/icaven/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [OBJ-Loader](https://github.com/Bly7/OBJ-Loader)
- [entt](https://github.com/skypjack/entt)

## Inspirations

- [Hazel](https://github.com/TheCherno/Hazel), game engine created by TheCherno.
- [Unreal Engine](https://www.unrealengine.com/)
- [javidx9 (YouTube)](https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA)
