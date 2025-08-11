#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include "Vroom/Render/Helpers/ResourcePool.h"

#include "Vroom/Render/Abstraction/Buffer.h"
#include "Vroom/Render/Abstraction/Texture.h"
#include "Vroom/Render/Abstraction/FrameBuffer.h"
#include "Vroom/Render/AutoBuffer.h"

namespace vrm
{
  
  class RenderResources
  {
  public:

    RenderResources();
    ~RenderResources();

    RenderResources& operator=(const RenderResources& other) = delete;
    RenderResources(const RenderResources& other) = delete;

    RenderResources& operator=(RenderResources&& other) = default;
    RenderResources(RenderResources&& other) = default;

    void clear();
    const std::vector<std::string>& getExposedTextures() const { return m_exposedTextures; }
    bool isTextureExposed(const std::string& name) const;

    gl::Buffer* genBuffer(const std::string& name);
    gl::Texture* genTexture(const std::string& name, bool expose = false);
    gl::FrameBuffer* genFramebuffer(const std::string& name);
    render::AutoBuffer* genAutoBuffer(const std::string& name);

    gl::Buffer* tryGetBuffer(const std::string& name);
    gl::Texture* tryGetTexture(const std::string& name);
    gl::FrameBuffer* tryGetFramebuffer(const std::string& name);
    render::AutoBuffer* tryGetAutoBuffer(const std::string& name);

    const gl::Buffer* tryGetBuffer(const std::string& name) const;
    const gl::Texture* tryGetTexture(const std::string& name) const;
    const gl::FrameBuffer* tryGetFramebuffer(const std::string& name) const;
    const render::AutoBuffer* tryGetAutoBuffer(const std::string& name) const;

  private:

    ResourcePool<gl::Buffer> m_buffers;
    ResourcePool<gl::Texture> m_textures;
    ResourcePool<gl::FrameBuffer> m_framebuffers;
    ResourcePool<render::AutoBuffer> m_autoBuffers;

    std::vector<std::string> m_exposedTextures;
    std::unordered_set<std::string> m_exposedTexturesSet;
  };

} // namespace vrm
