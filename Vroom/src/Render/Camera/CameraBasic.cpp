#include "Vroom/Render/Camera/CameraBasic.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Vroom/Asset/AssetData/MeshData.h"
#include "Vroom/Math/Aabb.h"

using namespace vrm;

CameraBasic::CameraBasic(float near, float far)
  : m_Near(near), m_Far(far)
{
  markViewDirty();
  markProjectionDirty();
}

void CameraBasic::setNear(float near)
{
  m_Near = near;
  markProjectionDirty();
}

void CameraBasic::setFar(float far)
{
  m_Far = far;
  markProjectionDirty();
}

const glm::mat4& CameraBasic::getView() const
{
  computeView();
  return m_View;
}

const glm::mat4& CameraBasic::getProjection() const
{
  computeProjection();
  return m_Projection;
}

const glm::mat4& CameraBasic::getViewProjection() const
{
  computeViewProjection();
  return m_ViewProjection;
}

glm::vec3 CameraBasic::getForwardVector() const
{
  auto& v = getView();
  return -glm::vec3(v[0][2], v[1][2], v[2][2]);
}

glm::vec3 CameraBasic::getUpVector() const
{
  auto& v = getView();
  return glm::vec3(v[0][1], v[1][1], v[2][1]);
}

glm::vec3 CameraBasic::getRightVector() const
{
  auto& v = getView();
  return glm::vec3(v[0][0], v[1][0], v[2][0]);
}

void CameraBasic::computeView() const
{
  if (!m_ViewDirty) return;

  m_View = onViewComputed();

  m_ViewDirty = false;
}

void CameraBasic::computeProjection() const
{
  if (!m_ProjectionDirty) return;

  m_Projection = onProjectionComputed();

  m_ProjectionDirty = false;
}

void CameraBasic::computeViewProjection() const
{
  if (!m_ViewProjectionDirty) return;

  m_ViewProjection = getProjection() * getView();

  m_ViewProjectionDirty = false;
}

MeshData CameraBasic::generateViewVolumeMesh() const
{
  std::vector<Vertex> vertices(6 * 4);
  std::vector<uint32_t> indices(6 * 2 * 3);

  Frustum frustum = Frustum::CreateFromAabb(Aabb::GetNDC(), true);
  glm::mat4 viewProjInv = glm::inverse(getViewProjection());

  frustum.transform(viewProjInv);

  auto corner = [&](int x, int y, int z) {
    return frustum.getCorner(
      x == 0 ? Frustum::eLeft : Frustum::eRight,
      y == 0 ? Frustum::eDown : Frustum::eUp,
      z == 0 ? Frustum::eNear : Frustum::eFar
    );
    };

  int v = 0;
  auto addQuad = [&](glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, int faceIdx) {
    int base = faceIdx * 4;
    vertices[base + 0].position = a;
    vertices[base + 1].position = b;
    vertices[base + 2].position = c;
    vertices[base + 3].position = d;

    glm::vec3 normal = glm::normalize(glm::cross(b - a, d - a));
    vertices[base + 0].normal = normal;
    vertices[base + 1].normal = normal;
    vertices[base + 2].normal = normal;
    vertices[base + 3].normal = normal;

    int i = faceIdx * 6;
    indices[i + 0] = base + 0;
    indices[i + 1] = base + 1;
    indices[i + 2] = base + 2;
    indices[i + 3] = base + 0;
    indices[i + 4] = base + 2;
    indices[i + 5] = base + 3;
    };

    // Near face
  addQuad(
    corner(0, 0, 0), // left-down-near
    corner(1, 0, 0), // right-down-near
    corner(1, 1, 0), // right-up-near
    corner(0, 1, 0), // left-up-near
    0
  );

  // Far face
  addQuad(
    corner(1, 0, 1), // right-down-far
    corner(0, 0, 1), // left-down-far
    corner(0, 1, 1), // left-up-far
    corner(1, 1, 1), // right-up-far
    1
  );

  // Left face
  addQuad(
    corner(0, 0, 1), // left-down-far
    corner(0, 0, 0), // left-down-near
    corner(0, 1, 0), // left-up-near
    corner(0, 1, 1), // left-up-far
    2
  );

  // Right face
  addQuad(
    corner(1, 0, 0), // right-down-near
    corner(1, 0, 1), // right-down-far
    corner(1, 1, 1), // right-up-far
    corner(1, 1, 0), // right-up-near
    3
  );

  // Top face
  addQuad(
    corner(0, 1, 0), // left-up-near
    corner(1, 1, 0), // right-up-near
    corner(1, 1, 1), // right-up-far
    corner(0, 1, 1), // left-up-far
    4
  );

  // Bottom face
  addQuad(
    corner(0, 0, 1), // left-down-far
    corner(1, 0, 1), // right-down-far
    corner(1, 0, 0), // right-down-near
    corner(0, 0, 0), // left-down-near
    5
  );

  return MeshData{
      std::move(vertices),
      std::move(indices)
  };
}