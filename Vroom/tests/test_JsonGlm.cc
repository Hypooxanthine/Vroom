#include <gtest/gtest.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <AssetManager/Json.h>
#include <AssetManager/JsonFile.h>

using json = nlohmann::json;
using namespace vrm;

TEST(test_JsonGlm, Vec3RoundTrip)
{
  glm::vec3 v(1.5f, -2.f, 3.25f);
  json j = v;

  ASSERT_TRUE(j.is_array());
  ASSERT_EQ(j.size(), 3u);

  glm::vec3 back = j.get<glm::vec3>();
  EXPECT_FLOAT_EQ(back.x, 1.5f);
  EXPECT_FLOAT_EQ(back.y, -2.f);
  EXPECT_FLOAT_EQ(back.z, 3.25f);
}

TEST(test_JsonGlm, Vec4RoundTrip)
{
  glm::vec4 v(1.f, 2.f, 3.f, 4.f);
  json j = v;

  glm::vec4 back = j.get<glm::vec4>();
  EXPECT_FLOAT_EQ(back.x, 1.f);
  EXPECT_FLOAT_EQ(back.w, 4.f);
}

TEST(test_JsonGlm, Mat4RoundTrip)
{
  glm::mat4 m(1.f);
  m[3] = glm::vec4(4.f, 5.f, 6.f, 1.f);
  json j = m;

  glm::mat4 back = j.get<glm::mat4>();
  for (int c = 0; c < 4; ++c)
    for (int r = 0; r < 4; ++r)
      EXPECT_FLOAT_EQ(back[c][r], m[c][r]);
}

TEST(test_JsonGlm, FromJsonRejectsNonArray)
{
  json j = 42;
  EXPECT_ANY_THROW((void)j.get<glm::vec3>());
}

TEST(test_JsonFile, WriteThenReadRoundTrip)
{
  std::filesystem::path path = std::filesystem::temp_directory_path() / "vroom_test_jsonfile.json";

  json j;
  j["int"] = 7;
  j["str"] = "hello";
  j["arr"] = { 1, 2, 3 };

  ASSERT_TRUE(WriteJsonFile(path, j));

  json out;
  ASSERT_TRUE(ReadJsonFile(path, out));
  EXPECT_EQ(out, j);

  std::error_code ec;
  std::filesystem::remove(path, ec);
}

TEST(test_JsonFile, ReadMissingFileReturnsFalse)
{
  json out;
  EXPECT_FALSE(ReadJsonFile("vroom_definitely_missing_file_98765.json", out));
}
