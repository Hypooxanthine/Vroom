#pragma once

#include <vector>

#include "Vroom/Render/RenderView.h"

namespace vrm
{
  
  class RenderLayout
  {
  public:

    /// @brief Will be normalized
    using RelativeSizes = std::vector<float>;

  public:

    RenderLayout();
    ~RenderLayout();

    /**
     * @brief Viewport sizes will be 1 / slices by default. Can be changed with setWidths and setHeights.
     * 
     * @param xSlices Number of columns
     * @param ySlices Number of rows
     */
    RenderLayout(size_t xSlices, size_t ySlices);

    RenderLayout& operator=(const RenderLayout& other) = default;
    RenderLayout(const RenderLayout& other) = default;

    RenderLayout& operator=(RenderLayout&& other) = default;
    RenderLayout(RenderLayout&& other) = default;

    inline size_t getXTiles() const { return m_widths.size(); }
    inline size_t getYTiles() const { return m_heights.size(); }
    bool isViewUsed(size_t xSlice, size_t ySlice) const;

    inline float getWidth(size_t xSlice) const { return m_widths.at(xSlice); }
    inline float getHeight(size_t ySlice) const { return m_heights.at(ySlice); }
    inline const RenderView& getView(size_t xSlice, size_t ySlice) const { return m_views.at(getViewIndex(xSlice, ySlice)); }

    void setWidths(const RelativeSizes& widths);
    void setHeights(const RelativeSizes& heights);
    void setView(size_t xSlice, size_t ySlice, const RenderView& view);

  private:

    static RelativeSizes NormalizedRelativeSizes(const RelativeSizes& sizesIn);

    inline RenderView& _getView(size_t xSlice, size_t ySlice) { return m_views.at(getViewIndex(xSlice, ySlice)); }

    inline size_t getViewIndex(size_t xSlice, size_t ySlice) const
    {
      return getXTiles() * ySlice + xSlice;
    }

    inline void updateViewports();

  private:

    std::vector<float> m_widths, m_heights;
    std::vector<RenderView> m_views;

  };

  inline void RenderLayout::updateViewports()
  {
    float cumulHeight = 0.f;

    for (size_t j = 0; j < getYTiles(); ++j)
    {
      const float height = m_heights.at(j);
      float cumulWidth = 0.f;

      for (size_t i = 0; i < getXTiles(); ++i)
      {
        const float width = m_widths.at(i);

        RenderView& view = _getView(i, j);

        NormalizedViewport vp = view.getViewport();

        vp.setOrigin({ cumulWidth, cumulHeight });
        vp.setSize({ width, height });
        view.setViewport(vp);

        cumulWidth += width;
      }

      cumulHeight += height;
    }
  }

} // namespace vrm
