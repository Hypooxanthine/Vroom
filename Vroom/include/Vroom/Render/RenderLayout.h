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

    inline RenderView& getView(size_t xSlice, size_t ySlice) { return m_views.at(getViewIndex(xSlice, ySlice)); }

    inline size_t getViewIndex(size_t xSlice, size_t ySlice) const
    {
      return getXTiles() * ySlice + xSlice;
    }

    template <bool updateWidths, bool updateHeights>
    inline void updateViewports();

  private:

    std::vector<float> m_widths, m_heights;
    std::vector<RenderView> m_views;

  };

  template <bool updateWidths, bool updateHeights>
  inline void RenderLayout::updateViewports()
  {
    if constexpr (!updateWidths && !updateHeights)
    {
      return;
    }

    for (size_t i = 0; i < m_views.size(); ++i)
    {
      RenderView& view = m_views[i];

      RenderViewport vp = view.getViewport();

      if constexpr (updateWidths)
      {
        const size_t xSlice = i % getXTiles();
        vp.setWidth(m_widths.at(xSlice));
      }

      if constexpr (updateHeights)
      {
        const size_t ySlice = i / getXTiles();
        vp.setHeight(m_heights.at(ySlice));
      }
    }
  }

} // namespace vrm
