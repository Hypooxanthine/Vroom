#pragma once

namespace vrm
{

class Application;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    /* Life cycle of a scene */

    /**
     * @brief Initialization of the scene.
     * 
     * @param app Application pointer.
     */
    void init(Application* app);

    /**
     * @brief Updates the scene.
     * 
     * @param dt Ellapsed time since last frame in seconds.
     */
    void update(float dt);

    /**
     * @brief Renders a frame of the scene.
     * 
     */
    void render();

    /**
     * @brief Ends the scene. Does all the necessarly cleanup.
     * 
     */
    void end();

protected:

    /**
     * @brief Allows extending scene behaviour by executing procedures at the end of the initialization step.
     * 
     */
    virtual void onInit() {}

    /**
     * @brief Allows extending scene behaviour by executing procedures at the end of the updating step.
     * 
     * @param dt Ellapsed time since last frame in seconds.
     */
    virtual void onUpdate(float dt) {}

    /**
     * @brief Allows extending scene behaviour by executing procedures at the end of the rendering step.
     * 
     */
    virtual void onRender() {}

    /**
     * @brief Allows extending scene behaviour by executing procedures at the end of the ending (cleanup) step.
     * 
     */
    virtual void onEnd() {}

    inline Application& getApplication() { return *m_Application; }
    inline const Application& getApplicaton() const { return *m_Application; }

private:

    Application* m_Application;

};

} // namespace vrm
