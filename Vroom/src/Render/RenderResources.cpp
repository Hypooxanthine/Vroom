#include "Vroom/Render/RenderResources.h"

using namespace vrm;

RenderResources::RenderResources()
{

}

RenderResources::~RenderResources()
{

}

void RenderResources::clear()
{
  m_buffers.clear();
  m_textures.clear();
  m_framebuffers.clear();
  m_autoBuffers.clear();
  m_exposedTextures.clear();
  m_exposedTexturesSet.clear();
}

bool RenderResources::isTextureExposed(const std::string& name) const
{
  return m_exposedTexturesSet.contains(name);
}

gl::Buffer* RenderResources::genBuffer(const std::string& name)
{
  return m_buffers.emplace(name);
}

gl::Texture* RenderResources::genTexture(const std::string& name, bool expose)
{
  gl::Texture* tex = m_textures.emplace(name);
  if (expose && tex)
  {
    m_exposedTextures.emplace_back(name);
    m_exposedTexturesSet.insert(name);
  }
  
  return tex;
}

gl::FrameBuffer* RenderResources::genFramebuffer(const std::string& name)
{
  return m_framebuffers.emplace(name);
}

render::AutoBuffer* RenderResources::genAutoBuffer(const std::string& name)
{
  return m_autoBuffers.emplace(name);
}

gl::Buffer* RenderResources::tryGetBuffer(const std::string& name)
{
  return m_buffers.tryGet(name);
}

gl::Texture* RenderResources::tryGetTexture(const std::string& name)
{
  return m_textures.tryGet(name);
}

gl::FrameBuffer* RenderResources::tryGetFramebuffer(const std::string& name)
{
  return m_framebuffers.tryGet(name);
}

render::AutoBuffer* RenderResources::tryGetAutoBuffer(const std::string& name)
{
  return m_autoBuffers.tryGet(name);
}

const gl::Buffer* RenderResources::tryGetBuffer(const std::string& name) const
{
  return const_cast<RenderResources*>(this)->tryGetBuffer(name);
}

const gl::Texture* RenderResources::tryGetTexture(const std::string& name) const
{
  return const_cast<RenderResources*>(this)->tryGetTexture(name);
}

const gl::FrameBuffer* RenderResources::tryGetFramebuffer(const std::string& name) const
{
  return const_cast<RenderResources*>(this)->tryGetFramebuffer(name);
}

const render::AutoBuffer* RenderResources::tryGetAutoBuffer(const std::string& name) const
{
  return const_cast<RenderResources*>(this)->tryGetAutoBuffer(name);
}

