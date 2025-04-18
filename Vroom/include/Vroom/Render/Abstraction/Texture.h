#pragma once

#include "Vroom/Core/Assert.h"
#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Asset/AssetData/TextureData.h"

namespace vrm::gl
{
  
  class Texture
  {
  public:

    inline static constexpr GLenum ToGlFormat(TextureFormat format)
    {
      switch (format)
      {
      case TextureFormat::Grayscale:
        return GL_RED;
      case TextureFormat::RG:
        return GL_RG;
      case TextureFormat::RGB:
        return GL_RGB;
      case TextureFormat::RGBA:
        return GL_RGBA;
      default:
        VRM_ASSERT_MSG(false, "Unsupported format.");
        return 0;
      }
    }

    inline static constexpr GLint ToGlInternalFormat(TextureFormat format)
    {
      switch (format)
      {
      case TextureFormat::Grayscale:
        return GL_R8;
      case TextureFormat::RG:
        return GL_RG8;
      case TextureFormat::RGB:
        return GL_RGB8;
      case TextureFormat::RGBA:
        return GL_RGBA8;
      default:
        VRM_ASSERT_MSG(false, "Unsupported format.");
        return 0;
      }
    }

    inline static constexpr GLenum GetBindingTarget(GLuint layers, GLuint samples)
    {
      if (layers > 1)
      {
        return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_ARRAY;
      }
      else
      {
        return samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
      }
    }

  };

} // namespace vrm::gl
