#include "ScriptEngine/ScriptEngine.h"
#include <cstdio>
#include <filesystem>
#include <memory>

#include "Core/Assert.h"
#include "Core/Log.h"
#include "ScriptEngine/RuntimeScriptLibrary.h"
#include "ScriptEngine/ScriptComponentPtr.h"
#include "ScriptEngine/ScriptHandle.h"

using namespace vrm;

std::unique_ptr<ScriptEngine> ScriptEngine::s_Instance = nullptr;

ScriptEngine::ScriptEngine()
{
  m_library = std::make_unique<RuntimeScriptLibrary>();
}

ScriptEngine::~ScriptEngine()
{
  _removeCopiedLibrary();
  m_libPath = std::filesystem::path{};
}

ScriptEngine& ScriptEngine::Get()
{
  if (!s_Instance)
  {
    [[unlikely]] s_Instance.reset(new ScriptEngine());
  }

  return *s_Instance;
}

bool ScriptEngine::loadScriptLibrary(const std::filesystem::path& libraryPath)
{
  std::filesystem::path copiedLibPath = _copyLibraryFile(libraryPath);

  if (copiedLibPath.empty())
    return false;

  std::unique_ptr<RuntimeScriptLibrary> tmpLib = std::make_unique<RuntimeScriptLibrary>();
  if (!tmpLib->load(copiedLibPath))
    return false;

  if (m_library->isLoaded())
  {
    // if (!_checkLibCompatible(*tmpLib))
    //   return false;

    // _refreshLoadedScriptsUncheck(*tmpLib);
    m_library->unload();
    _removeCopiedLibrary();
  }

  m_libPath = copiedLibPath;
  m_library = std::move(tmpLib);

  ++m_libraryGeneration;

  return true;
}

ScriptComponentPtr ScriptEngine::createScriptComponent(const std::string& scriptName)
{
  VRM_ASSERT_MSG(m_library->isLoaded(), "No pointer to the script constructor. Is the librairy loaded ?");
  VRM_ASSERT_MSG(m_library->scriptExists(scriptName), "Could not find {}", scriptName);

  ScriptHandle handle = m_library->createScript(scriptName);
  handle.setLibraryGeneration(m_libraryGeneration);
  return ScriptComponentPtr(handle);
}

std::vector<std::string> ScriptEngine::getRegisteredScriptIds() const
{
  if (!m_library->isLoaded())
    return {};

  return m_library->getScriptNames();
}

ScriptComponent* ScriptEngine::_getScript(ScriptHandle handle) const
{
  ScriptComponent* script = nullptr;

  if (m_library->isLoaded() && m_libraryGeneration == handle.getLibraryGeneration())
  {
    script = m_library->getScript(handle);
  }

  return script;
}

void ScriptEngine::_destroyScript(ScriptHandle handle)
{
  if (m_library->isLoaded() && handle.getLibraryGeneration() == m_libraryGeneration)
  {
    m_library->destroyScript(handle);
  }
}

std::filesystem::path ScriptEngine::_copyLibraryFile(const std::filesystem::path& libPath) const
{
  std::filesystem::path copiedLibPath = std::tmpnam(nullptr);

#if defined(VRM_PLATFORM_WINDOWS)
  copiedLibPath += ".dll";
#elif defined(VRM_PLATFORM_LINUX)
  copiedLibPath += ".so";
#endif

  if (std::filesystem::exists(copiedLibPath))
  {
    assert(false);
    VRM_LOG_ERROR("File name generation failed, {} was generated but already exists", copiedLibPath.string());
    return {};
  }

  try
  {
    std::filesystem::copy_file(libPath, copiedLibPath);
  } catch (std::filesystem::filesystem_error& e)
  {
    VRM_LOG_ERROR("Could not copy library {} to {}", libPath.string(), copiedLibPath.string());
    return {};
  }

#ifdef VRM_DEBUG
  std::filesystem::path symbolsPath = libPath;
  symbolsPath.replace_extension(std::filesystem::path{ "pdb" });

  if (std::filesystem::exists(symbolsPath))
  {
    std::filesystem::path copiedSymbolsPath = copiedLibPath;
    copiedSymbolsPath.replace_extension(std::filesystem::path{ "pdb" });

    try
    {
      std::filesystem::copy_file(symbolsPath, copiedSymbolsPath);
    } catch (std::filesystem::filesystem_error& e)
    {
      VRM_LOG_ERROR("Could not copy symbols {} to {}", symbolsPath.string(), copiedSymbolsPath.string());
      // Not false, but no debug symbols
    }
  }
#endif

  return copiedLibPath;
}

bool ScriptEngine::_removeCopiedLibrary() const
{
#ifdef VRM_DEBUG
  std::filesystem::path copiedSymbolsPath = m_libPath;
  copiedSymbolsPath.replace_extension(std::filesystem::path{ "pdb" });

  try
  {
    std::filesystem::remove(copiedSymbolsPath);
  } catch (...)
  {}
#endif

  try
  {
    std::filesystem::remove(m_libPath);
  } catch (...)
  {
    return false;
  }

  return true;
}

// bool ScriptEngine::_checkLibCompatible(const RuntimeScriptLibrary& lib) const
// {
//   auto& registry           = Application::Get().getGameLayer().getScene().getRegistry();
//   auto  scriptHandlersView = registry.view<ScriptHandler>();

//   for (auto entity : scriptHandlersView)
//   {
//     auto& scriptHandler = scriptHandlersView.get<ScriptHandler>(entity);

//     if (!scriptHandler.hasScript())
//       continue;

//     if (!_checkScriptsCompatibles(scriptHandler.getScript(), lib))
//       return false;
//   }

//   return true;
// }

// bool ScriptEngine::_checkScriptsCompatibles(const ScriptComponent& oldScript, const RuntimeScriptLibrary& newLib)
// const
// {
//   return true;
// }

// void ScriptEngine::_refreshLoadedScriptsUncheck(const RuntimeScriptLibrary& newLib) const
// {
//   auto& scene              = Application::Get().getGameLayer().getScene();
//   auto& registry           = scene.getRegistry();
//   auto  scriptHandlersView = registry.view<ScriptHandler>();

//   for (auto entity : scriptHandlersView)
//   {
//     auto& handler = scriptHandlersView.get<ScriptHandler>(entity);

//     if (!handler.hasScript())
//       continue;

//     ScriptComponentPtr newScript = newLib.createScript(handler.getScript().getScriptName());

//     if (newScript != nullptr)
//     {
//       const auto& oldScript = handler.getScript();

//       newScript->m_EntityHandle = oldScript.m_EntityHandle;
//       newScript->m_Scene        = oldScript.m_Scene;
//       newScript->m_scriptName   = oldScript.m_scriptName;
//       handler.getScriptEngineAttorney().replaceScript(std::move(newScript));
//     }
//     else
//     {
//       // Removing the script from the handler if we could not reload it
//       VRM_LOG_ERROR(
//         "A script named {} was beeing used but the new script library does not define it: removing the
//         ScriptComponent", handler.getScript().getScriptName());
//       registry.remove<ScriptHandler>(entity);
//     }
//   }
// }
