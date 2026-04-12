#include "Rasterizer/FrameBuffer.h"
#include <memory>

using namespace vrm::gl;

namespace
{

static std::unique_ptr<FrameBuffer> s_DefaultFrameBuffer = nullptr;

}

const FrameBuffer& FrameBuffer::GetDefaultFrameBuffer()
{
  if (s_DefaultFrameBuffer == nullptr)
  {
    s_DefaultFrameBuffer.reset(new FrameBuffer());
  }

  return *s_DefaultFrameBuffer;
}
