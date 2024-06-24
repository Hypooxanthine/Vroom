#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "Vroom/Scene/Entity.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"

namespace vrm
{

class Application;
class CameraBasic;
class AssetManager;

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
    
    
    /**
     * @brief Sets the camera of the scene.
     * @warning Scene does not take ownership of the camera, it only stores a pointer to it.
     * 
     * @param camera The camera to set.
     */
    inline void setCamera(CameraBasic* camera) { m_Camera = camera; }
    inline const CameraBasic& getCamera() const { return *m_Camera; }
    inline CameraBasic& getCamera() { return *m_Camera; }

    inline AssetManager& getAssetManager() { return *m_AssetManager; }
    inline const AssetManager& getAssetManager() const { return *m_AssetManager; }

    /**
     * @brief Creates an entity.
     * 
     * @param name The name of the entity.
     * @return Entity The created entity.
     */
    Entity createEntity(const std::string& name);

    /**
     * @brief Creates an entity with a default name.
     * 
     * @return Entity The created entity.
     */
    Entity createEntity();

    /**
     * @brief Checks if an entity exists by its name.
     * 
     * @param name The name of the entity.
     * @return true If the entity exists.
     * @return false If the entity does not exist.
     */
    bool entityExists(const std::string& name);

    /**
     * @brief Gets an entity by its handle.
     * 
     * @param handle The entity handle.
     * @return Entity The entity.
     */
    Entity getEntity(entt::entity handle);

    /**
     * @brief Gets an entity by its name.
     * 
     * @param name The name of the entity.
     * @return Entity The entity.
     */
    Entity getEntity(const std::string& name);

    /**
     * @brief Destroys an entity.
     * 
     * @param entity The entity to destroy.
     */
    void destroyEntity(Entity entity);


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
    std::unique_ptr<AssetManager> m_AssetManager;

    entt::registry m_Registry;
    size_t m_EntityCounter = 0;

    Application* m_Application;

    FirstPersonCamera m_DefaultCamera = FirstPersonCamera(0.1f, 100.f, 45.f, 600.f / 400.f, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 0.f, 0.f});
    CameraBasic* m_Camera;

};

} // namespace vrm
