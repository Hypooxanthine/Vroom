#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Renderer/Aabb.h>
#include <Renderer/Frustum.h>

using namespace vrm;

TEST(test_Aabb, AddPointsComputesMinMax)
{
  std::vector<glm::vec3> pts = { { 1.f, 2.f, 3.f }, { -1.f, 5.f, 0.f }, { 4.f, -2.f, 2.f } };
  Aabb box(pts.begin(), pts.end());

  EXPECT_FLOAT_EQ(box.getMin().x, -1.f);
  EXPECT_FLOAT_EQ(box.getMin().y, -2.f);
  EXPECT_FLOAT_EQ(box.getMin().z, 0.f);
  EXPECT_FLOAT_EQ(box.getMax().x, 4.f);
  EXPECT_FLOAT_EQ(box.getMax().y, 5.f);
  EXPECT_FLOAT_EQ(box.getMax().z, 3.f);
}

TEST(test_Aabb, Dimensions)
{
  Aabb box;
  box.addPoint({ 0.f, 0.f, 0.f });
  box.addPoint({ 2.f, 4.f, 6.f });

  EXPECT_FLOAT_EQ(box.calcWidth(), 2.f);
  EXPECT_FLOAT_EQ(box.calcHeight(), 4.f);
  EXPECT_FLOAT_EQ(box.calcDepth(), 6.f);

  glm::vec3 diag = box.calcMinToMaxVector();
  EXPECT_FLOAT_EQ(diag.x, 2.f);
  EXPECT_FLOAT_EQ(diag.y, 4.f);
  EXPECT_FLOAT_EQ(diag.z, 6.f);
}

TEST(test_Aabb, GetNDC)
{
  Aabb ndc = Aabb::GetNDC();
  EXPECT_FLOAT_EQ(ndc.getMin().x, -1.f);
  EXPECT_FLOAT_EQ(ndc.getMin().y, -1.f);
  EXPECT_FLOAT_EQ(ndc.getMin().z, -1.f);
  EXPECT_FLOAT_EQ(ndc.getMax().x, 1.f);
  EXPECT_FLOAT_EQ(ndc.getMax().y, 1.f);
  EXPECT_FLOAT_EQ(ndc.getMax().z, 1.f);
}

TEST(test_Aabb, CenterIsGeometricMidpoint)
{
  Aabb box;
  box.addPoint({ -2.f, -2.f, -2.f });
  box.addPoint({ 4.f, 4.f, 4.f });

  glm::vec3 center = box.calcCenter();
  EXPECT_FLOAT_EQ(center.x, 1.f);
  EXPECT_FLOAT_EQ(center.y, 1.f);
  EXPECT_FLOAT_EQ(center.z, 1.f);
}

TEST(test_Frustum, CornerIndexing)
{
  Frustum f;
  f.corner(Frustum::eLeft, Frustum::eDown, Frustum::eNear) = glm::vec3(1.f, 2.f, 3.f);

  EXPECT_EQ(f.getCorner(Frustum::eLeft, Frustum::eDown, Frustum::eNear), glm::vec3(1.f, 2.f, 3.f));
}

TEST(test_Frustum, CreateFromNdcAabbHasUnitCorners)
{
  Frustum f = Frustum::CreateFromAabb(Aabb::GetNDC(), /* fromNDC */ true);

  for (const glm::vec3& c : f.getCorners())
  {
    EXPECT_FLOAT_EQ(std::abs(c.x), 1.f);
    EXPECT_FLOAT_EQ(std::abs(c.y), 1.f);
    EXPECT_FLOAT_EQ(std::abs(c.z), 1.f);
  }
}

TEST(test_Frustum, AabbFromFrustumRoundTrip)
{
  Frustum f   = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  Aabb    box = Aabb::CreateFromFrustum(f);

  EXPECT_FLOAT_EQ(box.getMin().x, -1.f);
  EXPECT_FLOAT_EQ(box.getMin().z, -1.f);
  EXPECT_FLOAT_EQ(box.getMax().x, 1.f);
  EXPECT_FLOAT_EQ(box.getMax().z, 1.f);
}

TEST(test_Frustum, TransformTranslatesCorners)
{
  Frustum f = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  f.transform(glm::translate(glm::mat4(1.f), glm::vec3(10.f, 0.f, 0.f)));

  Aabb box = Aabb::CreateFromFrustum(f);
  EXPECT_FLOAT_EQ(box.getMin().x, 9.f);
  EXPECT_FLOAT_EQ(box.getMax().x, 11.f);
}
