#include "Vroom/Render/Abstraction/Texture.h"

using namespace vrm::gl;

Texture::Texture()
{

}

Texture::~Texture()
{
  release();
}

void Texture::release()
{
  if (m_renderId == 0) return;

  glDeleteTextures(1, &m_renderId);
  m_renderId = 0;
  m_defaultTarget = 0;
  m_desc = {};
}

void Texture::create(const Desc& desc)
{
  release();
  glGenTextures(1, &m_renderId);

  m_desc = desc;

  switch(desc.dimension)
  {
    case 1:
    {
      m_defaultTarget = desc.layered ? GL_TEXTURE_1D_ARRAY : GL_TEXTURE_1D ;
      bind();
      glTexStorage1D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width);
      break;
    }
    case 2:
    {
      if (desc.sampleCount == 1) // Not multisampled
      {
        if (desc.layered) // Array
        {
          if (desc.cubemap) // Cubemap array
          {
            m_defaultTarget = GL_TEXTURE_CUBE_MAP_ARRAY;
            bind();
            glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
          }
          else // 2D texture array
          {
            m_defaultTarget = GL_TEXTURE_2D_ARRAY;
            bind();
            glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
          }
        }
        else
        {
          if (desc.cubemap) // Cubemap
          {
            m_defaultTarget = GL_TEXTURE_CUBE_MAP;
            bind();
            glTexStorage2D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height);
          }
          else // 2D texture
          {
            m_defaultTarget = GL_TEXTURE_2D;
            bind();
            glTexStorage2D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height);
          }
        }
      }
      else // Multisampled
      {
        if (desc.layered) // Multisample 2D texture array
        {
          m_defaultTarget = GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
          bind();
          glTexStorage3DMultisample(m_defaultTarget, desc.sampleCount, desc.internalFormat, desc.width, desc.height, desc.depth, desc.fixedSampleLocations);
        }
        else // Multisample 2D texture
        {
          m_defaultTarget = GL_TEXTURE_2D_MULTISAMPLE;
          bind();
          glTexStorage2DMultisample(m_defaultTarget, desc.sampleCount, desc.internalFormat, desc.width, desc.height, desc.fixedSampleLocations);
        }
      }
      break;
    }
    case 3:
    {
      if (desc.sampleCount == 1)
      {
        m_defaultTarget = GL_TEXTURE_3D;
        bind();
        glTexStorage3D(m_defaultTarget, desc.mipmapCount, desc.internalFormat, desc.width, desc.height, desc.depth);
      }
      break;
    }
  }
}

void Texture::bind(GLenum target) const
{
  glBindTexture(target, m_renderId);
}

void Texture::bind() const
{
  bind(m_defaultTarget);
}


