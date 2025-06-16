#include "Vroom/Render/Abstraction/FrameBuffer.h"

using namespace vrm::gl;

inline std::unique_ptr<FrameBuffer> FrameBuffer::s_DefaultFrameBuffer = nullptr;