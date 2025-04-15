#pragma once

namespace vrm
{

  class RenderPass
  {
  public:
    RenderPass();
    virtual ~RenderPass();

    void init();

    void setup();

    void render() const;

    void cleanup();

  protected:
    /**
     * @brief Called once at the creation of the render pass
     *
     */
    virtual void onInit() {}

    /**
     * @brief Called for all at the setup stage of the frame
     *
     */
    virtual void onSetup() {}

    /**
     * @brief Called for all passes at the rendering stage of the frame
     * 
     */
    virtual void onRender() const = 0;

    /**
     * @brief Called for all passes at the cleanup stage of the frame
     * 
     */

    virtual void onCleanup() {}

  private:
  };

} // namespace vrm
