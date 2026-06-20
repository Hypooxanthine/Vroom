#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Scene/TransformComponent.h>

using namespace vrm;

namespace
{
void expectVec3Near(const glm::vec3& a, const glm::vec3& b, float eps = 1e-4f)
{
  EXPECT_NEAR(a.x, b.x, eps);
  EXPECT_NEAR(a.y, b.y, eps);
  EXPECT_NEAR(a.z, b.z, eps);
}
} // namespace

TEST(test_TransformComponent, Defaults)
{
  TransformComponent tc;
  expectVec3Near(tc.getPosition(), glm::vec3(0.f));
  expectVec3Near(tc.getRotation(), glm::vec3(0.f));
  expectVec3Near(tc.getScale(), glm::vec3(1.f));
}

TEST(test_TransformComponent, SettersGetters)
{
  TransformComponent tc;
  tc.setPosition({ 1.f, 2.f, 3.f });
  tc.setScale({ 2.f, 3.f, 4.f });

  expectVec3Near(tc.getPosition(), glm::vec3(1.f, 2.f, 3.f));
  expectVec3Near(tc.getScale(), glm::vec3(2.f, 3.f, 4.f));
}

TEST(test_TransformComponent, RotationWrapsIntoZeroTwoPi)
{
  TransformComponent tc;
  const float twoPi = glm::two_pi<float>();

  tc.setRotation({ 0.f, 0.f, twoPi + 0.5f });
  EXPECT_NEAR(tc.getRotation().z, 0.5f, 1e-4f);

  tc.setRotation({ 0.f, 0.f, -0.5f });
  EXPECT_NEAR(tc.getRotation().z, twoPi - 0.5f, 1e-4f);
}

TEST(test_TransformComponent, DirtyFlagSetOnChangeClearedOnGet)
{
  TransformComponent tc;

  tc.setPosition({ 1.f, 0.f, 0.f });
  EXPECT_TRUE(tc.isTransformDirty());

  (void)tc.getTransform();
  EXPECT_FALSE(tc.isTransformDirty());
}

TEST(test_TransformComponent, TransformContainsTranslationAndScale)
{
  TransformComponent tc;
  tc.setPosition({ 1.f, 2.f, 3.f });
  tc.setScale({ 2.f, 2.f, 2.f });

  const glm::mat4& m = tc.getTransform();

  // Translation lives in the 4th column.
  expectVec3Near(glm::vec3(m[3]), glm::vec3(1.f, 2.f, 3.f));
  // No rotation -> scale on the diagonal.
  EXPECT_NEAR(m[0][0], 2.f, 1e-4f);
  EXPECT_NEAR(m[1][1], 2.f, 1e-4f);
  EXPECT_NEAR(m[2][2], 2.f, 1e-4f);
}

TEST(test_TransformComponent, RotationAboutZMapsXAxisToYAxis)
{
  TransformComponent tc;
  tc.setRotation({ 0.f, 0.f, glm::half_pi<float>() });

  glm::vec3 rotated = glm::vec3(tc.getTransform() * glm::vec4(1.f, 0.f, 0.f, 1.f));
  expectVec3Near(rotated, glm::vec3(0.f, 1.f, 0.f));
}

TEST(test_TransformComponent, SetTransformDecomposesBackToComponents)
{
  TransformComponent tc;
  glm::mat4 t = glm::translate(glm::mat4(1.f), glm::vec3(5.f, 6.f, 7.f));

  tc.setTransform(t);

  expectVec3Near(tc.getPosition(), glm::vec3(5.f, 6.f, 7.f));
  expectVec3Near(tc.getScale(), glm::vec3(1.f, 1.f, 1.f));
  EXPECT_FALSE(tc.isTransformDirty());
}
