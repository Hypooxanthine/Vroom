#include "VroomEditor/UserInterface/AssetBrowser/AssetUtils.h"

#include <fstream>
#include <thread>

#include "Vroom/Asset/Parsing/Json.h"

#include "VroomEditor/UserInterface/AssetBrowser/AssetElement.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFile.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetDirectory.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileSceneAsset.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMeshAsset.h"
#include "VroomEditor/UserInterface/AssetBrowser/AssetFileMaterialAsset.h"

using namespace vrm;

MetaFile::MetaFile(const std::filesystem::path& metaPath, const std::filesystem::path& filePath)
  : metaPath(metaPath), filePath(filePath)
{
  if (!load())
  {
    type = EType::eNone;
  }
}

bool MetaFile::load()
{
  std::ifstream ifs;
  ifs.open(metaPath);

  VRM_CHECK_RET_FALSE_MSG(ifs.is_open(), "Could not open .meta file: {}", metaPath.string());

  json j;
  try
  {
    ifs >> j;
  }
  catch(const std::exception& e)
  {
    VRM_CHECK_RET_FALSE_MSG(false, "Could not parse .meta json content: {}. Parsing error:\n{}", metaPath.string(), e.what());
  }

  CHECK_OBJECT(j);
  CHECK_ATTR_STRING(j, Type);
  
  static const std::unordered_map<std::string, EType> typeTable =
  {
    { "Scene",    EType::eScene },
    { "Mesh",     EType::eMesh },
    { "Material", EType::eMaterial },
  };

  CHECK(typeTable.contains(TypeVal), "Unknown file type: {}", TypeVal);

  type = typeTable.at(TypeVal);

  return true;
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

  std::filesystem::path metaDataPath = path;
  metaDataPath += ".meta";

  if (!std::filesystem::exists(metaDataPath))
  {
    return std::make_unique<AssetFile>(path);
  }

  return CreateAssetElement(MetaFile{ metaDataPath, path });
}

std::unique_ptr<AssetElement> AssetUtils::CreateAssetElement(const MetaFile& meta)
{
  switch (meta.type)
  {
  case MetaFile::EType::eScene:
    return std::make_unique<AssetFileSceneAsset>(meta.filePath);
  case MetaFile::EType::eMesh:
    return std::make_unique<AssetFileMeshAsset>(meta.filePath);
  case MetaFile::EType::eMaterial:
    return std::make_unique<AssetFileMaterialAsset>(meta.filePath);
  case MetaFile::EType::eNone:
  default:
    return nullptr;
  }
}

static void OpenNativeFileExplorer_Impl(const std::filesystem::path& path)
{
  std::string strPath = std::filesystem::absolute(path).string();
  std::string cmd;
#if defined(_WIN32)
  cmd = "explorer " + strPath;
#elif defined(__UNIX__)
  cmd = "open " + strPath;
#else
  return;
#endif
  system(cmd.c_str());
}

void AssetUtils::OpenNativeFileExplorer(const std::filesystem::path& path)
{
  std::thread(OpenNativeFileExplorer_Impl, path).detach();
}
