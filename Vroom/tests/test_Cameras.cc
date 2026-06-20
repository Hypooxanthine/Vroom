#include <gtest/gtest.h>
#include <glm/glm.hpp>

#include <Renderer/FirstPersonCamera.h>
#include <Renderer/OrthographicCamera.h>

using namespace vrm;

namespace
{
void expectVec3Near(const glm::vec3& a, const glm::vec3& b, float eps = 1e-4f)
{
  EXPECT_NEAR(a.x, b.x, eps);
  EXPECT_NEAR(a.y, b.y, eps);
  EXPECT_NEAR(a.z, b.z, eps);
}

void expectMat4Near(const glm::mat4& a, const glm::mat4& b, float eps = 1e-4f)
{
  for (int c = 0; c < 4; ++c)
    for (int r = 0; r < 4; ++r)
      EXPECT_NEAR(a[c][r], b[c][r], eps);
}

FirstPersonCamera makeFpc(const glm::vec3& pos = glm::vec3(0.f), const glm::vec3& rot = glm::vec3(0.f))
{
  return FirstPersonCamera(0.1f, 100.f, 90.f, 1.f, pos, rot);
}
} // namespace

TEST(test_Camera, NearFarSetters)
{
  FirstPersonCamera cam = makeFpc();
  EXPECT_FLOAT_EQ(cam.getNear(), 0.1f);
  EXPECT_FLOAT_EQ(cam.getFar(), 100.f);

  cam.setNear(0.5f);
  cam.setFar(50.f);
  EXPECT_FLOAT_EQ(cam.getNear(), 0.5f);
  EXPECT_FLOAT_EQ(cam.getFar(), 50.f);
}

TEST(test_Camera, FirstPersonDefaultBasisVectors)
{
  FirstPersonCamera cam = makeFpc();
  expectVec3Near(cam.getForwardVector(), glm::vec3(0.f, 0.f, -1.f));
  expectVec3Near(cam.getUpVector(), glm::vec3(0.f, 1.f, 0.f));
  expectVec3Near(cam.getRightVector(), glm::vec3(1.f, 0.f, 0.f));
}

TEST(test_Camera, ViewMapsCameraPositionToOrigin)
{
  FirstPersonCamera cam = makeFpc(glm::vec3(3.f, 4.f, 5.f), glm::vec3(0.3f, 0.5f, 0.f));
  glm::vec4 originVS = cam.getView() * glm::vec4(cam.getPosition(), 1.f);
  expectVec3Near(glm::vec3(originVS), glm::vec3(0.f));
}

TEST(test_Camera, ViewProjectionIsProjectionTimesView)
{
  FirstPersonCamera cam = makeFpc(glm::vec3(1.f, 2.f, 3.f), glm::vec3(0.1f, 0.2f, 0.f));
  expectMat4Near(cam.getViewProjection(), cam.getProjection() * cam.getView());
}

TEST(test_Camera, MoveAndYawUpdatePose)
{
  FirstPersonCamera cam = makeFpc();

  cam.setWorldPosition(glm::vec3(1.f, 2.f, 3.f));
  expectVec3Near(cam.getPosition(), glm::vec3(1.f, 2.f, 3.f));

  cam.move(glm::vec3(1.f, 0.f, 0.f));
  expectVec3Near(cam.getPosition(), glm::vec3(2.f, 2.f, 3.f));

  cam.addYaw(0.5f);
  EXPECT_FLOAT_EQ(cam.getRotation().y, 0.5f);
}

TEST(test_Camera, OrthographicViewAndProjection)
{
  OrthographicCamera cam(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f);

  cam.setWorldPosition(glm::vec3(5.f, 0.f, 0.f));
  expectVec3Near(cam.getPosition(), glm::vec3(5.f, 0.f, 0.f));

  glm::vec4 originVS = cam.getView() * glm::vec4(cam.getPosition(), 1.f);
  expectVec3Near(glm::vec3(originVS), glm::vec3(0.f));

  expectMat4Near(cam.getViewProjection(), cam.getProjection() * cam.getView());
}
