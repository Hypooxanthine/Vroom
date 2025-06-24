#pragma once

#include <memory>
#include <array>
#include <filesystem>

#include <Vroom/Event/CustomEvent/CustomEventManager.h>

#include "VroomEditor/UserInterface/ImGuiElement.h"
#include "Vroom/Core/Layer.h"
#include "VroomEditor/UserInterface/OSFileDrop.h"

struct ImFont;
struct ImGuiStyle;

namespace vrm
{

  class MainMenuBar;
  class StatisticsPanel;
  class RenderSettingsPanel;
  class Viewport;
  class AssetBrowser;
  class SceneGraph;
  class EditorPreferences;

  struct EInterfaceElement
  {
    enum Element : size_t
    {
      // Enums must be contiguous
      eViewport = 0,
      eMainMenuBar,
      eStatisticsPanel,
      eRenderSettingsPanel,
      eSceneGraph,
      eAssetBrowser,
      eEntityEditor,
      eEditorPreferences,
      eElementCount
    };
  };

  class UserInterfaceLayer : public Layer
  {
  public:

    inline static const std::filesystem::path s_imguiStyleFile = "Resources/Editor/Appearance/Style/imguiStyle.json";

    struct ViewportInfos
    {
      bool justChangedSize = false;
      int width;
      int height;
      bool active = false;
      bool playing = false;
      bool simulating = false;
      bool paused = false;
      bool manipulatingGuizmo = false;
      bool localSpace = false;
      glm::ivec2 clickPos = { -1, -1 };
    };

  public:
    UserInterfaceLayer();
    virtual ~UserInterfaceLayer();

    static UserInterfaceLayer& Get();

    inline const auto& getViewportInfo() const { return m_ViewportInfo; }
    inline auto& getViewportInfo() { return m_ViewportInfo; }
    inline void setViewportInfos(const ViewportInfos& viewportInfo) { m_ViewportInfo = viewportInfo; }

    inline void setElementVisible(EInterfaceElement::Element element, bool visible) { m_openFlags.at(element) = visible; }
    inline bool isElementVisible(EInterfaceElement::Element element) const { return m_openFlags.at(element); }

    void saveImguiStyle(const ImGuiStyle& style) const;

    inline ImGuiElement& getElement(EInterfaceElement::Element elem) { return *m_elements.at(elem); }
    Viewport& getViewport();

  protected:
    virtual void onInit() override;
    virtual void onEnd() override;
    virtual void onUpdate(const DeltaTime& dt) override;
    virtual void onRender() override;
    virtual void onEvent(vrm::Event& e) override;

  private:
    template <typename T, typename... Args>
    T& emplaceImGuiElement(EInterfaceElement::Element elem, Args&&... args, bool open = true)
    {
      auto& uniquePtr = m_elements.at(elem) = std::make_unique<T>(std::forward<Args>(args)...);
      setElementVisible(elem, open);
      uniquePtr->setOpenBoolean(&m_openFlags.at(elem));
      return static_cast<T&>(*uniquePtr.get());
    }

    void renderImgui();

    void fileDropCallback(const Event& e);

    bool _loadImguiStyle();

  private:
    CustomEventManager m_CustomEventManager;

    ImFont* m_Font;

    std::array<std::unique_ptr<ImGuiElement>, EInterfaceElement::eElementCount> m_elements = {};
    std::array<bool, EInterfaceElement::eElementCount> m_openFlags;
    bool m_isDraggingOSFile = false;
    OSFileDrop m_fileDrop;
    std::string m_fileDropData;

    ViewportInfos m_ViewportInfo;
  };

} // namespace vrm
