
# Vroom

Vroom is a 3D game engine made with OpenGL. It is a personal project I'm doing for fun to improve myself as a C++ developer.
This project has been reset, the old version in 2D with SFML is still available in branch "V1".

## Building project

Building has only been tested on Windows.
In repository root folder, open a terminal and type :

```bash
mkdir build
cd build
cmake ..
cmake --build . --config [Release|Debug] # If you don't want to open Visual Studio solution
ctest # If you want to run unit tests
doxygen # If you need to generate documentation
```

Output files will be written in Vroom/build/[Project]/[config]/ folder, where [Project] is Vroom or Sandbox, and config is Release or Debug.

You can also compile by opening the root folder on VS Code, and use the "CMake" and "CMake Tools" VS Code extensions (that is my workflow).

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

## Known problems

- Unbinding trigger and custom event inputs is buggy.
