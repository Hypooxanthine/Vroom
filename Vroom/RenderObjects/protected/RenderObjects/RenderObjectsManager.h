#pragma once

#include "RenderObjects/Api.h"
#include "ShaderManager.h"


namespace vrm::render
{

/**
 * @brief Just a singleton storage class, storing all render objects managers.
 *
 */
class VRM_RENDEROBJECTS_API RenderObjectsManager
{
public:

  static void                  Init();
  static void                  Shutdown();
  static RenderObjectsManager& Get();

public:

  ShaderManager& getShaderManager();

private:

  // Singleton class -> private constructor
  RenderObjectsManager() = default;

private:

  ShaderManager m_shaderManager;
};

} // namespace vrm::render
