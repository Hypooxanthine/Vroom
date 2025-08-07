#include "Vroom/Render/RenderLayout.h"

#include <numeric>

#include "Vroom/Core/Assert.h"

using namespace vrm;

RenderLayout::RenderLayout()
  : RenderLayout(1, 1)
{

}

RenderLayout::~RenderLayout()
{

}

RenderLayout::RenderLayout(size_t xSlices, size_t ySlices)
{
  VRM_ASSERT_MSG(xSlices > 0 && ySlices > 0, "RenderLayout slices cannot be zero");

  m_widths.assign(xSlices, 1.f / static_cast<float>(xSlices));
  m_heights.assign(ySlices, 1.f / static_cast<float>(ySlices));
  m_views.resize(xSlices * ySlices, RenderView(nullptr, NormalizedViewport()));
  updateViewports();
}

bool RenderLayout::isViewUsed(size_t xSlice, size_t ySlice) const
{
  return getView(xSlice, ySlice).getCamera() != nullptr;
}

void RenderLayout::setWidths(const RelativeSizes& widths)
{
  VRM_ASSERT_MSG(widths.size() == getXTiles(), "{} relative sizes were given, but {} were expected", widths.size(), getXTiles());
  m_widths = NormalizedRelativeSizes(widths);
  updateViewports();
}

void RenderLayout::setHeights(const RelativeSizes& heights)
{
  VRM_ASSERT_MSG(heights.size() == getYTiles(), "{} relative sizes were given, but {} were expected", heights.size(), getYTiles());
  m_heights = NormalizedRelativeSizes(heights);
  updateViewports();
}

void RenderLayout::setView(size_t xSlice, size_t ySlice, const RenderView& view)
{
  NormalizedViewport viewport = view.getViewport();
  auto& storedView = _getView(xSlice, ySlice);
  storedView = view;
  storedView.setViewport(viewport);
  updateViewports();
}

RenderLayout::RelativeSizes RenderLayout::NormalizedRelativeSizes(const RelativeSizes& sizesIn)
{
  const float sum = std::accumulate(sizesIn.cbegin(), sizesIn.cend(), 0.f);
  VRM_ASSERT_MSG(sum > 0.0001f, "Relative sizes are zero or too small");
  const auto divisor = [=](float val) { return val / sum; };

  RelativeSizes sizesOut(sizesIn.size(), 0.f);
  std::transform(sizesIn.cbegin(), sizesIn.cend(), sizesOut.begin(), divisor);
  return sizesOut;
}
