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

std::filesystem::path AssetUtils::GetMetaName(const std::filesystem::path& baseName)
{
  std::filesystem::path meta = baseName;
  meta += ".meta";
  return meta;
}

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

  std::filesystem::path metaDataPath = GetMetaName(path);

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
  std::filesystem::path metaPath = GetMetaName(filePath);
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

bool AssetUtils::CreateDirectory(const std::filesystem::path& path)
{
  try
  {
    std::filesystem::create_directory(path);
  }
  catch(const std::exception& e)
  {
    return false;
  }

  return true;
}

bool AssetUtils::DeleteAssetElement(const std::filesystem::path& path)
{
  namespace fs = std::filesystem;

  try
  {
    fs::path metaPath = GetMetaName(path);

    if (fs::exists(metaPath))
    {
      std::filesystem::remove(metaPath);
    }

    if (fs::exists(path))
    {
      fs::remove_all(path);
    }
  }
  catch(const std::exception& e)
  {
    return false;
  }

  return true;
}

bool AssetUtils::RenameAssetElement(const std::filesystem::path& assetPath, const std::filesystem::path& targetName)
{
  namespace fs = std::filesystem;

  try
  {
    fs::path metaPath = GetMetaName(assetPath);

    fs::rename(assetPath, targetName);

    if (fs::exists(metaPath))
    {
      fs::path newMetaPath = GetMetaName(targetName);
      fs::rename(metaPath, newMetaPath);
    }
  }
  catch(const std::exception& e)
  {
    return false;
  }

  return true;
}
