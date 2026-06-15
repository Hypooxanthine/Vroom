#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>

#include "AssetManager/Api.h"

namespace vrm
{

  /**
   * @brief Opens @p path and parses its content as JSON into @p out.
   *
   * On failure (file cannot be opened or does not contain valid JSON), logs an
   * error and returns false. @p out is left untouched in that case.
   *
   * @return true if the file was read and parsed successfully.
   */
  VRM_ASSETMANAGER_API bool ReadJsonFile(const std::filesystem::path& path, nlohmann::json& out);

  /**
   * @brief Serializes @p j to @p path, truncating any existing content.
   *
   * @param indent Indentation forwarded to nlohmann's dump(). Use a negative
   *               value for compact (single-line) output.
   *
   * On failure (file cannot be opened or serialization throws), logs an error
   * and returns false.
   *
   * @return true if the file was written successfully.
   */
  VRM_ASSETMANAGER_API bool WriteJsonFile(const std::filesystem::path& path, const nlohmann::json& j, int indent = 2);

} // namespace vrm
