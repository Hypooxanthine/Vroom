#include "Editor/AssetUtils.h"
#include <filesystem>
#include <string>
#include <thread>

#include "Editor/AssetDirectory.h"
#include "Editor/AssetElement.h"
#include "Editor/AssetFile.h"
#include "Editor/AssetFileMaterialAsset.h"
#include "Editor/AssetFileMeshAsset.h"
#include "Editor/AssetFileSceneAsset.h"

#include "AssetManager/Json.h"
#include "AssetManager/JsonFile.h"
#include "Core/Log.h"
#include "Editor/AssetFileTextureAsset.h"

using namespace vrm;

std::filesystem::path AssetUtils::GetMetaName(const std::filesystem::path& baseName)
{
  std::filesystem::path meta  = baseName;
  meta                       += ".meta";
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

  json j;
  if (!ReadJsonFile(metaDataPath, j))
    return nullptr;

  MetaFile metaData = j;
  return CreateAssetElement(metaData, path);
}

std::unique_ptr<AssetElement> AssetUtils::CreateAssetElement(const MetaFile&              meta,
                                                             const std::filesystem::path& filePath)
{
  switch (meta.Type)
  {
  case MetaFile::EType::eScene:
    return std::make_unique<AssetFileSceneAsset>(filePath);
  case MetaFile::EType::eMesh:
    return std::make_unique<AssetFileMeshAsset>(filePath);
  case MetaFile::EType::eMaterial:
    return std::make_unique<AssetFileMaterialAsset>(filePath);
  case MetaFile::EType::eTexture:
    return std::make_unique<AssetFileTextureAsset>(filePath);
  case MetaFile::EType::eNone:
  default:
    return nullptr;
  }
}

bool AssetUtils::CreateMetaFile(const MetaFile& meta, const std::filesystem::path& filePath)
{
  std::filesystem::create_directory(filePath.parent_path());
  std::filesystem::path metaPath = GetMetaName(filePath);

  json j = meta;
  if (!WriteJsonFile(metaPath, j))
  {
    VRM_LOG_WARN("Could not create meta file {} for file {}.", metaPath.string(), filePath.string());
    return false;
  }

  return true;
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

  std::string strPath  = std::filesystem::absolute(path).string();
  cmd                 += " \"" + strPath + "\"";

  system(cmd.c_str());
}

void AssetUtils::OpenNativeFileExplorer(const std::filesystem::path& path)
{
  std::thread(OpenNativeFileExplorer_Impl, path).detach();
}

std::filesystem::path AssetUtils::FindFreeAssetName(const std::filesystem::path& basePath)
{
  if (!basePath.has_filename())
  {
    return {};
  }

  size_t                i             = 1;
  std::filesystem::path currentPath   = basePath;
  std::string           baseStem      = basePath.stem().string();
  std::string           baseExtension = basePath.extension().string();

  while (std::filesystem::exists(currentPath))
  {
    ++i;
    std::filesystem::path newFileName = baseStem + "_" + std::to_string(i) + baseExtension;
    currentPath.replace_filename(newFileName);
  }

  return currentPath;
}

bool AssetUtils::CreateDirectory(const std::filesystem::path& path)
{
  try
  {
    std::filesystem::create_directory(path);
  } catch (const std::exception& e)
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
  } catch (const std::exception& e)
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
  } catch (const std::exception& e)
  {
    return false;
  }

  return true;
}
