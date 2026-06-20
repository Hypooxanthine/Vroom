#include <gtest/gtest.h>
#include <memory>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <AssetManager/ComponentData.h>

using namespace vrm;

TEST(test_ComponentData, TransformRoundTrip)
{
  TransformComponentData a;
  a.position = { 1.f, 2.f, 3.f };
  a.rotation = { 0.25f, 0.5f, 0.75f };
  a.scale    = { 2.f, 4.f, 8.f };

  nlohmann::json j = a.serialize();

  TransformComponentData b;
  ASSERT_TRUE(b.deserialize(j));

  EXPECT_FLOAT_EQ(b.position.x, 1.f);
  EXPECT_FLOAT_EQ(b.position.y, 2.f);
  EXPECT_FLOAT_EQ(b.position.z, 3.f);
  EXPECT_FLOAT_EQ(b.rotation.x, 0.25f);
  EXPECT_FLOAT_EQ(b.rotation.y, 0.5f);
  EXPECT_FLOAT_EQ(b.rotation.z, 0.75f);
  EXPECT_FLOAT_EQ(b.scale.x, 2.f);
  EXPECT_FLOAT_EQ(b.scale.y, 4.f);
  EXPECT_FLOAT_EQ(b.scale.z, 8.f);
}

TEST(test_ComponentData, MeshRoundTrip)
{
  MeshComponentData a;
  a.resourceName = "MyMesh";
  a.castsShadow  = true;
  a.visible      = false;

  nlohmann::json j = a.serialize();

  MeshComponentData b;
  ASSERT_TRUE(b.deserialize(j));

  EXPECT_EQ(b.resourceName, "MyMesh");
  EXPECT_TRUE(b.castsShadow);
  EXPECT_FALSE(b.visible);
}

TEST(test_ComponentData, DirectionalLightRoundTrip)
{
  DirectionalLightComponentData a;
  a.color        = { 0.5f, 0.25f, 1.f };
  a.intensity    = 2.5f;
  a.castsShadows = true;

  nlohmann::json j = a.serialize();

  DirectionalLightComponentData b;
  ASSERT_TRUE(b.deserialize(j));

  EXPECT_FLOAT_EQ(b.color.x, 0.5f);
  EXPECT_FLOAT_EQ(b.color.y, 0.25f);
  EXPECT_FLOAT_EQ(b.color.z, 1.f);
  EXPECT_FLOAT_EQ(b.intensity, 2.5f);
  EXPECT_TRUE(b.castsShadows);
}

TEST(test_ComponentData, PointLightRoundTrip)
{
  PointLightComponentData a;
  a.color                = { 0.5f, 0.25f, 1.f };
  a.intensity            = 3.f;
  a.radius               = 10.f;
  a.smoothRadius         = 2.f;
  a.constantAttenuation  = 1.f;
  a.linearAttenuation    = 0.5f;
  a.quadraticAttenuation = 0.25f;

  nlohmann::json j = a.serialize();

  PointLightComponentData b;
  ASSERT_TRUE(b.deserialize(j));

  EXPECT_FLOAT_EQ(b.intensity, 3.f);
  EXPECT_FLOAT_EQ(b.radius, 10.f);
  EXPECT_FLOAT_EQ(b.smoothRadius, 2.f);
  EXPECT_FLOAT_EQ(b.constantAttenuation, 1.f);
  EXPECT_FLOAT_EQ(b.linearAttenuation, 0.5f);
  EXPECT_FLOAT_EQ(b.quadraticAttenuation, 0.25f);
}

TEST(test_ComponentData, CloneIsIndependentCopy)
{
  TransformComponentData a;
  a.position = { 5.f, 6.f, 7.f };

  std::unique_ptr<ComponentData> c(a.clone());
  auto* tc = dynamic_cast<TransformComponentData*>(c.get());
  ASSERT_NE(tc, nullptr);
  EXPECT_FLOAT_EQ(tc->position.x, 5.f);

  tc->position.x = 99.f;
  EXPECT_FLOAT_EQ(a.position.x, 5.f); // original unchanged
}

TEST(test_ComponentData, DeserializeRejectsNonObject)
{
  TransformComponentData a;
  nlohmann::json notAnObject = nlohmann::json::array();
  EXPECT_FALSE(a.deserialize(notAnObject));
}

TEST(test_ComponentData, DeserializeRejectsUnknownParameter)
{
  TransformComponentData a;
  nlohmann::json j;
  j["type"]            = "TransformComponent";
  j["params"]          = nlohmann::json::array();
  nlohmann::json param = nlohmann::json::object();
  param["name"]        = "NotARealParameter";
  param["value"]       = nlohmann::json::array({ 0, 0, 0 });
  j["params"].push_back(param);

  EXPECT_FALSE(a.deserialize(j));
}
