#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"

#include <fstream>
#include <thread>

#include "Vroom/Core/Log.h"

#include "Vroom/Asset/Parsing/Json.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetDirectory.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileSceneAsset.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMeshAsset.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMaterialAsset.h"

using namespace vrm;

std::unique_ptr<AssetElement> AssetUtils::CreateAssetElement(const std::filesystem::path& path)
{
  // Directories
  if (std::filesystem::is_directory(path))
  {
    return std::make_unique<AssetDirectory>(path);
  }

  if (!std::filesystem::is_regular_file(path))
  {
    return nullptr;
  }

  // For files, we check .meta files (we do not add .meta files in the browser)
  if (path.has_extension() && path.extension() == ".meta")
  {
    return nullptr;
  }

  std::filesystem::path metaDataPath = path;
  metaDataPath += ".meta";

  if (!std::filesystem::exists(metaDataPath))
  {
    return std::make_unique<AssetFile>(path);
  }

  std::ifstream ifs(metaDataPath);
  if (ifs.is_open())
  {
    json j;
    ifs >> j;
    MetaFile metaData = j;
    return CreateAssetElement(metaData, path);
  }
  else
  {
    VRM_LOG_WARN("Meta file {} exists but could not open it", metaDataPath.string());
    return nullptr;
  }

}

std::unique_ptr<AssetElement> AssetUtils::CreateAssetElement(const MetaFile& meta, const std::filesystem::path& filePath)
{
  switch (meta.Type)
  {
  case MetaFile::EType::eScene:
    return std::make_unique<AssetFileSceneAsset>(filePath);
  case MetaFile::EType::eMesh:
    return std::make_unique<AssetFileMeshAsset>(filePath);
  case MetaFile::EType::eMaterial:
    return std::make_unique<AssetFileMaterialAsset>(filePath);
  case MetaFile::EType::eNone:
  default:
    return nullptr;
  }
}

bool AssetUtils::CreateMetaFile(const MetaFile& meta, const std::filesystem::path& filePath)
{
  std::ofstream ofs;
  std::filesystem::path metaPath = filePath;
  metaPath += ".meta";
  ofs.open(metaPath, std::fstream::trunc);

  if (ofs.is_open())
  {
    json j = meta;
    ofs << j;

    return true;
  }
  else
  {
    VRM_LOG_WARN("Could not create meta file {} for file {}.", metaPath.string(), filePath.string());
    return false;
  }
}

static void OpenNativeFileExplorer_Impl(const std::filesystem::path& path)
{
  std::string cmd;

#if defined(_WIN32)
  cmd = "explorer";
#elif defined(__APPLE__)
  cmd = "open";
#elif defined(__linux__)
  cmd = "xdg-open";
#else
  VRM_LOG_WARN("Unsupported platform");
  return;
#endif

  std::string strPath = std::filesystem::absolute(path).string();
  cmd += " \"" + strPath + "\"";

  system(cmd.c_str());
}

void AssetUtils::OpenNativeFileExplorer(const std::filesystem::path& path)
{
  std::thread(OpenNativeFileExplorer_Impl, path).detach();
}
