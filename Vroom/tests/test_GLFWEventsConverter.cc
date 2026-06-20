#include <gtest/gtest.h>

#include <Event/Codes.h>
#include <Event/GLFWEventsConverter.h>

using namespace vrm;

class GLFWEventsConverterTest : public testing::Test
{
protected:
  void SetUp() override { GLFWEventsConverter::Init(); }
};

TEST_F(GLFWEventsConverterTest, KeyCodeRoundTrip)
{
  const KeyCode codes[] = { KeyCode::A,  KeyCode::Z,         KeyCode::Space,
                            KeyCode::Enter, KeyCode::Up,      KeyCode::Down,
                            KeyCode::LeftShift, KeyCode::NUMPAD_0 };

  for (KeyCode code : codes)
  {
    int glfw = GLFWEventsConverter::GetGLFWFromKeyCode(code);
    EXPECT_EQ(GLFWEventsConverter::GetKeyCodeFromGLFW(glfw), code);
  }
}

TEST_F(GLFWEventsConverterTest, MouseCodeRoundTrip)
{
  const MouseCode codes[] = { MouseCode::Left, MouseCode::Right, MouseCode::Middle,
                              MouseCode::MB4,  MouseCode::MB8 };

  for (MouseCode code : codes)
  {
    int glfw = GLFWEventsConverter::GetGLFWFromMouseCode(code);
    EXPECT_EQ(GLFWEventsConverter::GetMouseCodeFromGLFW(glfw), code);
  }
}

TEST_F(GLFWEventsConverterTest, UnknownGLFWKeyReturnsNone)
{
  EXPECT_EQ(GLFWEventsConverter::GetKeyCodeFromGLFW(-9999), KeyCode::None);
}

TEST_F(GLFWEventsConverterTest, UnsupportedKeyCodeThrows)
{
  EXPECT_ANY_THROW(GLFWEventsConverter::GetGLFWFromKeyCode(KeyCode::None));
}
