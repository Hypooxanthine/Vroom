#include <gtest/gtest.h>
#include <Vroom/Render/RenderLayout.h>
#include <Vroom/Render/Camera/CameraBasic.h>

class TestRenderLayout : public testing::Test
{
protected:

  void SetUp() override
  {
    // Use dummy pointer values for testing
    camera = reinterpret_cast<vrm::CameraBasic*>(0x12345678);
    camera2 = reinterpret_cast<vrm::CameraBasic*>(0x87654321);
  }

  void TearDown() override
  {

  }

  vrm::CameraBasic* camera;
  vrm::CameraBasic* camera2;
};

TEST_F(TestRenderLayout, DefaultConstructor)
{
  vrm::RenderLayout layout;

  EXPECT_EQ(layout.getCols(), 1);
  EXPECT_EQ(layout.getRows(), 1);
}

TEST_F(TestRenderLayout, ParameterizedConstructor)
{
  vrm::RenderLayout layout(2, 3);

  EXPECT_EQ(layout.getCols(), 3);
  EXPECT_EQ(layout.getRows(), 2);

  // Check default normalized widths and heights
  for (size_t i = 0; i < 3; ++i) {
    EXPECT_FLOAT_EQ(layout.getWidth(i), 1.0f / 3.0f);
  }
  for (size_t j = 0; j < 2; ++j) {
    EXPECT_FLOAT_EQ(layout.getHeight(j), 1.0f / 2.0f);
  }
}

TEST_F(TestRenderLayout, SetWidths)
{
  vrm::RenderLayout layout(2, 3);
  vrm::RenderLayout::RelativeSizes widths = { 1.0f, 2.0f, 1.0f }; // Should normalize to {0.25, 0.5, 0.25}

  layout.setWidths(widths);

  EXPECT_FLOAT_EQ(layout.getWidth(0), 0.25f);
  EXPECT_FLOAT_EQ(layout.getWidth(1), 0.5f);
  EXPECT_FLOAT_EQ(layout.getWidth(2), 0.25f);
}

TEST_F(TestRenderLayout, SetHeights)
{
  vrm::RenderLayout layout(3, 2);
  vrm::RenderLayout::RelativeSizes heights = { 3.0f, 1.0f, 2.0f }; // Should normalize to {0.5, 1/6, 1/3}

  layout.setHeights(heights);

  EXPECT_FLOAT_EQ(layout.getHeight(0), 0.5f);
  EXPECT_FLOAT_EQ(layout.getHeight(1), 1.0f / 6.0f);
  EXPECT_FLOAT_EQ(layout.getHeight(2), 1.0f / 3.0f);
}

TEST_F(TestRenderLayout, SetAndGetView)
{
  vrm::RenderLayout layout(2, 2);
  vrm::render::NormalizedViewport viewport({ 0.1f, 0.2f }, { 0.3f, 0.4f });
  vrm::render::NormalizedView view(camera, viewport);

  layout.setView(0, 1, view);

  const vrm::render::NormalizedView& retrievedView = layout.getView(0, 1);
  EXPECT_EQ(retrievedView.getCamera(), camera);

  const auto& retrievedViewport = retrievedView.getViewport();
  EXPECT_FLOAT_EQ(retrievedViewport.getOriginX(), 0.5f);
  EXPECT_FLOAT_EQ(retrievedViewport.getOriginY(), 0.f);
  EXPECT_FLOAT_EQ(retrievedViewport.getWidth(), 0.5f);
  EXPECT_FLOAT_EQ(retrievedViewport.getHeight(), 0.5f);
}

TEST_F(TestRenderLayout, ViewportCalculation)
{
  vrm::RenderLayout layout(2, 2);

  // Set custom widths and heights
  vrm::RenderLayout::RelativeSizes widths = { 1.0f, 3.0f }; // Normalized: {0.25, 0.75}
  vrm::RenderLayout::RelativeSizes heights = { 2.0f, 1.0f }; // Normalized: {2/3, 1/3}

  layout.setWidths(widths);
  layout.setHeights(heights);

  // Create views for each position
  vrm::render::NormalizedView view(camera);

  for (size_t j = 0; j < 2; ++j) {
    for (size_t i = 0; i < 2; ++i) {
      layout.setView(i, j, view);
    }
  }

  // Check viewport calculations
  // View (0,0): origin (0, 0), size (0.25, 2/3)
  const auto& view00 = layout.getView(0, 0);
  const auto& vp00 = view00.getViewport();
  EXPECT_FLOAT_EQ(vp00.getOriginX(), 0.0f);
  EXPECT_FLOAT_EQ(vp00.getOriginY(), 0.0f);
  EXPECT_FLOAT_EQ(vp00.getWidth(), 0.25f);
  EXPECT_FLOAT_EQ(vp00.getHeight(), 2.0f / 3.0f);

  // View (0,1): origin (0.25, 0), size (0.75, 2/3)
  const auto& view01 = layout.getView(0, 1);
  const auto& vp01 = view01.getViewport();
  EXPECT_FLOAT_EQ(vp01.getOriginX(), 0.25f);
  EXPECT_FLOAT_EQ(vp01.getOriginY(), 0.0f);
  EXPECT_FLOAT_EQ(vp01.getWidth(), 0.75f);
  EXPECT_FLOAT_EQ(vp01.getHeight(), 2.0f / 3.0f);

  // View (1,0): origin (0, 2/3), size (0.25, 1/3)
  const auto& view10 = layout.getView(1, 0);
  const auto& vp10 = view10.getViewport();
  EXPECT_FLOAT_EQ(vp10.getOriginX(), 0.0f);
  EXPECT_FLOAT_EQ(vp10.getOriginY(), 2.0f / 3.0f);
  EXPECT_FLOAT_EQ(vp10.getWidth(), 0.25f);
  EXPECT_FLOAT_EQ(vp10.getHeight(), 1.0f / 3.0f);

  // View (1,1): origin (0.25, 2/3), size (0.75, 1/3)
  const auto& view11 = layout.getView(1, 1);
  const auto& vp11 = view11.getViewport();
  EXPECT_FLOAT_EQ(vp11.getOriginX(), 0.25f);
  EXPECT_FLOAT_EQ(vp11.getOriginY(), 2.0f / 3.0f);
  EXPECT_FLOAT_EQ(vp11.getWidth(), 0.75f);
  EXPECT_FLOAT_EQ(vp11.getHeight(), 1.0f / 3.0f);
}

TEST_F(TestRenderLayout, CopyConstructor)
{
  vrm::RenderLayout layout1(2, 2);
  vrm::RenderLayout::RelativeSizes widths = { 1.0f, 2.0f };
  layout1.setWidths(widths);

  vrm::RenderLayout layout2(layout1);

  EXPECT_EQ(layout2.getCols(), 2);
  EXPECT_EQ(layout2.getRows(), 2);
  EXPECT_FLOAT_EQ(layout2.getWidth(0), 1.0f / 3.0f);
  EXPECT_FLOAT_EQ(layout2.getWidth(1), 2.0f / 3.0f);
}

TEST_F(TestRenderLayout, MoveConstructor)
{
  vrm::RenderLayout layout1(1, 3);
  vrm::RenderLayout::RelativeSizes widths = { 1.0f, 2.0f, 3.0f };
  layout1.setWidths(widths);

  vrm::RenderLayout layout2(std::move(layout1));

  EXPECT_EQ(layout2.getCols(), 3);
  EXPECT_EQ(layout2.getRows(), 1);
  EXPECT_FLOAT_EQ(layout2.getWidth(0), 1.0f / 6.0f);
  EXPECT_FLOAT_EQ(layout2.getWidth(1), 2.0f / 6.0f);
  EXPECT_FLOAT_EQ(layout2.getWidth(2), 3.0f / 6.0f);
}

TEST_F(TestRenderLayout, AssignmentOperator)
{
  vrm::RenderLayout layout1(3, 2);
  vrm::RenderLayout layout2;

  vrm::RenderLayout::RelativeSizes heights = { 1.0f, 1.0f, 2.0f };
  layout1.setHeights(heights);

  layout2 = layout1;

  EXPECT_EQ(layout2.getCols(), 2);
  EXPECT_EQ(layout2.getRows(), 3);
  EXPECT_FLOAT_EQ(layout2.getHeight(0), 0.25f);
  EXPECT_FLOAT_EQ(layout2.getHeight(1), 0.25f);
  EXPECT_FLOAT_EQ(layout2.getHeight(2), 0.5f);
}