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
     */
    RenderLayout(size_t rows, size_t cols);

    RenderLayout& operator=(const RenderLayout& other) = default;
    RenderLayout(const RenderLayout& other) = default;

    RenderLayout& operator=(RenderLayout&& other) = default;
    RenderLayout(RenderLayout&& other) = default;

    inline size_t getCols() const { return m_widths.size(); }
    inline size_t getRows() const { return m_heights.size(); }
    bool isViewUsed(size_t row, size_t col) const;

    inline float getWidth(size_t col) const { return m_widths.at(col); }
    inline float getHeight(size_t row) const { return m_heights.at(row); }
    inline const RenderView& getView(size_t row, size_t col) const { return m_views.at(getViewIndex(row, col)); }

    void setWidths(const RelativeSizes& widths);
    void setHeights(const RelativeSizes& heights);
    void setView(size_t row, size_t col, const RenderView& view);

  private:

    static RelativeSizes NormalizedRelativeSizes(const RelativeSizes& sizesIn);

    inline RenderView& _getView(size_t row, size_t col) { return m_views.at(getViewIndex(row, col)); }

    inline size_t getViewIndex(size_t row, size_t col) const
    {
      return getCols() * row + col;
    }

    inline void updateViewports();

  private:

    std::vector<float> m_widths, m_heights;
    std::vector<RenderView> m_views;

  };

  inline void RenderLayout::updateViewports()
  {
    float cumulHeight = 0.f;

    for (size_t j = 0; j < getRows(); ++j)
    {
      const float height = getHeight(j);
      float cumulWidth = 0.f;

      for (size_t i = 0; i < getCols(); ++i)
      {
        const float width = getWidth(i);

        RenderView& view = _getView(j, i);

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
