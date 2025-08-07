#include <gtest/gtest.h>
#include <Vroom/Render/RenderLayout.h>

class TestRenderLayout : public testing::Test
{
protected:

  void SetUp() override
  {

  }

  void TearDown() override
  {

  }
};

TEST_F(TestRenderLayout, DefaultConstructor)
{
  vrm::RenderLayout layout;

  EXPECT_EQ(layout.getXTiles(), 1);
  EXPECT_EQ(layout.getYTiles(), 1);
  EXPECT_FLOAT_EQ(layout.getWidth(0), 1.0f);
  EXPECT_FLOAT_EQ(layout.getHeight(0), 1.0f);
}

TEST_F(TestRenderLayout, ParameterizedConstructor)
{
  vrm::RenderLayout layout(3, 2);

  EXPECT_EQ(layout.getXTiles(), 3);
  EXPECT_EQ(layout.getYTiles(), 2);

  // Check that widths are evenly distributed
  EXPECT_FLOAT_EQ(layout.getWidth(0), 1.0f / 3.0f);
  EXPECT_FLOAT_EQ(layout.getWidth(1), 1.0f / 3.0f);
  EXPECT_FLOAT_EQ(layout.getWidth(2), 1.0f / 3.0f);

  // Check that heights are evenly distributed
  EXPECT_FLOAT_EQ(layout.getHeight(0), 1.0f / 2.0f);
  EXPECT_FLOAT_EQ(layout.getHeight(1), 1.0f / 2.0f);
}

TEST_F(TestRenderLayout, CopyConstructor)
{
  vrm::RenderLayout original(2, 3);
  vrm::RenderLayout copy(original);

  EXPECT_EQ(copy.getXTiles(), original.getXTiles());
  EXPECT_EQ(copy.getYTiles(), original.getYTiles());

  for (size_t i = 0; i < original.getXTiles(); ++i)
  {
    EXPECT_FLOAT_EQ(copy.getWidth(i), original.getWidth(i));
  }

  for (size_t i = 0; i < original.getYTiles(); ++i)
  {
    EXPECT_FLOAT_EQ(copy.getHeight(i), original.getHeight(i));
  }
}

TEST_F(TestRenderLayout, MoveConstructor)
{
  vrm::RenderLayout original(2, 3);
  size_t originalXTiles = original.getXTiles();
  size_t originalYTiles = original.getYTiles();

  vrm::RenderLayout moved(std::move(original));

  EXPECT_EQ(moved.getXTiles(), originalXTiles);
  EXPECT_EQ(moved.getYTiles(), originalYTiles);
}

TEST_F(TestRenderLayout, AssignmentOperator)
{
  vrm::RenderLayout layout1(2, 2);
  vrm::RenderLayout layout2(3, 4);

  layout1 = layout2;

  EXPECT_EQ(layout1.getXTiles(), layout2.getXTiles());
  EXPECT_EQ(layout1.getYTiles(), layout2.getYTiles());

  for (size_t i = 0; i < layout2.getXTiles(); ++i)
  {
    EXPECT_FLOAT_EQ(layout1.getWidth(i), layout2.getWidth(i));
  }

  for (size_t i = 0; i < layout2.getYTiles(); ++i)
  {
    EXPECT_FLOAT_EQ(layout1.getHeight(i), layout2.getHeight(i));
  }
}

TEST_F(TestRenderLayout, MoveAssignmentOperator)
{
  vrm::RenderLayout layout1(2, 2);
  vrm::RenderLayout layout2(3, 4);

  size_t originalXTiles = layout2.getXTiles();
  size_t originalYTiles = layout2.getYTiles();

  layout1 = std::move(layout2);

  EXPECT_EQ(layout1.getXTiles(), originalXTiles);
  EXPECT_EQ(layout1.getYTiles(), originalYTiles);
}

TEST_F(TestRenderLayout, SetWidths)
{
  vrm::RenderLayout layout(3, 2);

  std::vector<float> widths = { 0.5f, 0.3f, 0.2f };
  layout.setWidths(widths);

  EXPECT_FLOAT_EQ(layout.getWidth(0), 0.5f);
  EXPECT_FLOAT_EQ(layout.getWidth(1), 0.3f);
  EXPECT_FLOAT_EQ(layout.getWidth(2), 0.2f);
}

TEST_F(TestRenderLayout, SetWidthsNormalization)
{
  vrm::RenderLayout layout(3, 2);

  // Widths that don't sum to 1.0
  std::vector<float> widths = { 2.0f, 3.0f, 5.0f }; // Sum = 10.0f
  layout.setWidths(widths);

  EXPECT_FLOAT_EQ(layout.getWidth(0), 0.2f);  // 2.0/10.0
  EXPECT_FLOAT_EQ(layout.getWidth(1), 0.3f);  // 3.0/10.0
  EXPECT_FLOAT_EQ(layout.getWidth(2), 0.5f);  // 5.0/10.0
}

TEST_F(TestRenderLayout, SetHeights)
{
  vrm::RenderLayout layout(2, 3);

  std::vector<float> heights = { 0.6f, 0.3f, 0.1f };
  layout.setHeights(heights);

  EXPECT_FLOAT_EQ(layout.getHeight(0), 0.6f);
  EXPECT_FLOAT_EQ(layout.getHeight(1), 0.3f);
  EXPECT_FLOAT_EQ(layout.getHeight(2), 0.1f);
}

TEST_F(TestRenderLayout, SetHeightsNormalization)
{
  vrm::RenderLayout layout(2, 3);

  // Heights that don't sum to 1.0
  std::vector<float> heights = { 1.0f, 2.0f, 3.0f }; // Sum = 6.0f
  layout.setHeights(heights);

  EXPECT_FLOAT_EQ(layout.getHeight(0), 1.0f / 6.0f);
  EXPECT_FLOAT_EQ(layout.getHeight(1), 2.0f / 6.0f);
  EXPECT_FLOAT_EQ(layout.getHeight(2), 3.0f / 6.0f);
}

TEST_F(TestRenderLayout, IsViewUsed)
{
  vrm::RenderLayout layout(2, 2);

  // Initially, all views should have nullptr camera (not used)
  EXPECT_FALSE(layout.isViewUsed(0, 0));
  EXPECT_FALSE(layout.isViewUsed(0, 1));
  EXPECT_FALSE(layout.isViewUsed(1, 0));
  EXPECT_FALSE(layout.isViewUsed(1, 1));
}

TEST_F(TestRenderLayout, SetViewAndIsViewUsed)
{
  vrm::RenderLayout layout(2, 2);

  // Initially, all views should have nullptr camera (not used)
  EXPECT_FALSE(layout.isViewUsed(0, 0));
  EXPECT_FALSE(layout.isViewUsed(0, 1));
  EXPECT_FALSE(layout.isViewUsed(1, 0));
  EXPECT_FALSE(layout.isViewUsed(1, 1));

  // Create a mock camera and view
  int mockCamera = 42;
  vrm::RenderView view(reinterpret_cast<vrm::CameraBasic*>(&mockCamera), vrm::RenderViewport());

  layout.setView(0, 0, view);

  // The view should now be considered used
  EXPECT_TRUE(layout.isViewUsed(0, 0));
  EXPECT_FALSE(layout.isViewUsed(0, 1));
  EXPECT_FALSE(layout.isViewUsed(1, 0));
  EXPECT_FALSE(layout.isViewUsed(1, 1));
}

TEST_F(TestRenderLayout, MultipleViewsUsage)
{
  vrm::RenderLayout layout(3, 2);

  // Set up views for different positions
  int camera1 = 1, camera2 = 2, camera3 = 3;

  vrm::RenderView view1(reinterpret_cast<vrm::CameraBasic*>(&camera1), vrm::RenderViewport());
  vrm::RenderView view2(reinterpret_cast<vrm::CameraBasic*>(&camera2), vrm::RenderViewport());
  vrm::RenderView view3(reinterpret_cast<vrm::CameraBasic*>(&camera3), vrm::RenderViewport());

  layout.setView(0, 0, view1);
  layout.setView(1, 1, view2);
  layout.setView(2, 0, view3);

  // Check which views are used
  EXPECT_TRUE(layout.isViewUsed(0, 0));
  EXPECT_FALSE(layout.isViewUsed(1, 0));
  EXPECT_TRUE(layout.isViewUsed(2, 0));
  EXPECT_FALSE(layout.isViewUsed(0, 1));
  EXPECT_TRUE(layout.isViewUsed(1, 1));
  EXPECT_FALSE(layout.isViewUsed(2, 1));
}

TEST_F(TestRenderLayout, BoundaryConditions)
{
  // Test single tile layout
  vrm::RenderLayout layout1x1(1, 1);
  EXPECT_EQ(layout1x1.getXTiles(), 1);
  EXPECT_EQ(layout1x1.getYTiles(), 1);
  EXPECT_FLOAT_EQ(layout1x1.getWidth(0), 1.0f);
  EXPECT_FLOAT_EQ(layout1x1.getHeight(0), 1.0f);

  // Test asymmetric layouts
  vrm::RenderLayout layout1x5(1, 5);
  EXPECT_EQ(layout1x5.getXTiles(), 1);
  EXPECT_EQ(layout1x5.getYTiles(), 5);
  EXPECT_FLOAT_EQ(layout1x5.getWidth(0), 1.0f);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_FLOAT_EQ(layout1x5.getHeight(i), 0.2f);
  }

  vrm::RenderLayout layout5x1(5, 1);
  EXPECT_EQ(layout5x1.getXTiles(), 5);
  EXPECT_EQ(layout5x1.getYTiles(), 1);
  EXPECT_FLOAT_EQ(layout5x1.getHeight(0), 1.0f);
  for (size_t i = 0; i < 5; ++i) {
    EXPECT_FLOAT_EQ(layout5x1.getWidth(i), 0.2f);
  }
}
