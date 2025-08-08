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

RenderLayout::RenderLayout(size_t rows, size_t cols)
{
  VRM_ASSERT_MSG(rows > 0 && cols > 0, "RenderLayout rows or cols cannot be zero");

  m_heights.assign(rows, 1.f / static_cast<float>(rows));
  m_widths.assign(cols, 1.f / static_cast<float>(cols));
  m_views.resize(rows * cols, render::NormalizedView(nullptr, render::NormalizedViewport()));
  updateViewports();
}

bool RenderLayout::isViewUsed(size_t row, size_t col) const
{
  return getView(row, col).getCamera() != nullptr;
}

void RenderLayout::setWidths(const RelativeSizes& widths)
{
  VRM_ASSERT_MSG(widths.size() == getCols(), "{} relative sizes were given, but {} were expected", widths.size(), getCols());
  m_widths = NormalizedRelativeSizes(widths);
  updateViewports();
}

void RenderLayout::setHeights(const RelativeSizes& heights)
{
  VRM_ASSERT_MSG(heights.size() == getRows(), "{} relative sizes were given, but {} were expected", heights.size(), getRows());
  m_heights = NormalizedRelativeSizes(heights);
  updateViewports();
}

void RenderLayout::setView(size_t row, size_t col, const render::NormalizedView& view)
{
  render::NormalizedViewport viewport = view.getViewport();
  auto& storedView = _getView(row, col);
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
