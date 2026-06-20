#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include <AssetManager/MaterialData.h>

using namespace vrm;
using Param = MaterialData::Parameter;

TEST(test_MaterialData, TypeSetGet)
{
  MaterialData m;
  EXPECT_EQ(m.getType(), MaterialData::EType::eUndefined);

  m.setType(MaterialData::EType::eShadingModel);
  EXPECT_EQ(m.getType(), MaterialData::EType::eShadingModel);
}

TEST(test_MaterialData, ShadingModelNameLookup)
{
  MaterialData m;
  m.setType(MaterialData::EType::eShadingModel);
  m.setShadingModel(MaterialData::EShadingModel::ePhong);

  EXPECT_EQ(m.getShadingModel(), MaterialData::EShadingModel::ePhong);
  EXPECT_EQ(m.getShadingModelName(), "Phong");
}

TEST(test_MaterialData, SetParameterAndQuery)
{
  MaterialData m;

  Param p("roughness");
  p.type = Param::eFloat;
  p.setValue(0.5f);
  m.setParameter(p);

  EXPECT_TRUE(m.hasParameter("roughness"));
  EXPECT_FALSE(m.hasParameter("missing"));
  EXPECT_FLOAT_EQ(m.getParameter("roughness").getValue<float>(), 0.5f);
}

TEST(test_MaterialData, SetParameterWithoutTypeIsIgnored)
{
  MaterialData m;

  Param p("bad"); // type defaults to eNone
  m.setParameter(p);

  EXPECT_FALSE(m.hasParameter("bad"));
}

TEST(test_MaterialData, TextureCountTracksSamplers)
{
  MaterialData m;
  EXPECT_EQ(static_cast<int>(m.getTextureCount()), 0);

  Param tex("albedo");
  tex.type = Param::eSampler2D;
  tex.setValue(MaterialData::TextureData{});
  m.setParameter(tex);
  EXPECT_EQ(static_cast<int>(m.getTextureCount()), 1);

  // Overwriting the same sampler must not increase the count again.
  Param tex2("albedo");
  tex2.type = Param::eSampler2D;
  tex2.setValue(MaterialData::TextureData{});
  m.setParameter(tex2);
  EXPECT_EQ(static_cast<int>(m.getTextureCount()), 1);

  // Replacing the sampler with a non-sampler parameter must decrease it.
  Param notTex("albedo");
  notTex.type = Param::eFloat;
  notTex.setValue(1.f);
  m.setParameter(notTex);
  EXPECT_EQ(static_cast<int>(m.getTextureCount()), 0);
}

TEST(test_MaterialData, JsonRoundTrip)
{
  MaterialData m;
  m.setType(MaterialData::EType::eShadingModel);
  m.setShadingModel(MaterialData::EShadingModel::ePhong);

  Param p("roughness");
  p.type = Param::eFloat;
  p.setValue(0.5f);
  m.setParameter(p);

  nlohmann::json j = m;
  MaterialData parsed = j.get<MaterialData>();

  EXPECT_EQ(parsed.getType(), MaterialData::EType::eShadingModel);
  EXPECT_EQ(parsed.getShadingModel(), MaterialData::EShadingModel::ePhong);
  ASSERT_TRUE(parsed.hasParameter("roughness"));
  EXPECT_EQ(parsed.getParameter("roughness").type, Param::eFloat);
  EXPECT_FLOAT_EQ(parsed.getParameter("roughness").getValue<float>(), 0.5f);
}
