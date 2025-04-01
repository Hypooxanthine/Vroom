#pragma once

#include <memory>
#include <unordered_map>
#include <entt/entt.hpp>

#include "Vroom/Scene/Entity.h"
#include "Vroom/Render/Camera/FirstPersonCamera.h"

#include "Vroom/Asset/StaticAsset/SceneAsset.h"

namespace vrm
{

class Application;
class CameraBasic;
class AssetManager;
class SceneData;

class Scene
{
public:
    friend class Renderer;
public:
    Scene();
    virtual ~Scene();

    /* Life cycle of a scene */

    /**
     * @brief Initialization of the scene.
     * 
     */
    void init();

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
     * @brief Loads the scene from file.
     * 
     * @param sceneAsset Scene asset handle
     */
    bool loadFromAsset(const SceneAsset::Handle& sceneAsset);
    
    
    /**
     * @brief Sets the camera of the scene.
     * @warning Scene does not take ownership of the camera, it only stores a pointer to it.
     * 
     * @param camera The camera to set.
     */
    inline void setCamera(CameraBasic* camera) { m_Camera = camera; }
    inline const CameraBasic& getCamera() const { return *m_Camera; }
    inline CameraBasic& getCamera() { return *m_Camera; }

    entt::registry& getRegistry() { return m_Registry; }
    const entt::registry& getRegistry() const { return m_Registry; }

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

    void renameEntity(Entity& e, const std::string& name);

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
     * @brief Gets scene root entity.
     * 
     * @return Entity The entity.
     */
    inline Entity& getRoot() { return m_Root; }

    /**
     * @brief Check if "parent" is the parent of "child" 
     */
    bool checkEntitiesRelation(const Entity& parent, const Entity& child) const;

    /**
     * @brief Check if "ancestor" is an ancestor of "child"
     */
    bool checkEntityAncestor(const Entity& ancestor, const Entity& child) const;

    void setEntitiesRelation(Entity& parent, Entity& child);

    /**
     * @brief Destroys an entity.
     * 
     * @param entity The entity to destroy.
     */
    void destroyEntity(Entity& entity);

    /**
     * @brief Destroys all entities.
     * 
     */
    void destroyAllEntities();

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

private:

    /**
     * @brief Creating an registering a raw entity. Unsafe, internal use only.
     * 
     * @return Entity Created entity.
     */
    Entity createRawEntity(const std::string& nameTag);

    Entity createRoot();

    void renameRoot(const std::string& rootName);

    void destroyEntityRecursive(Entity entity);

    bool loadFromAsset2(const SceneData& data);

private:
    entt::registry m_Registry;
    size_t m_EntityCounter = 0;
    std::unordered_map<std::string, Entity> m_EntitiesByName;

    static FirstPersonCamera s_DefaultCamera;
    CameraBasic* m_Camera;

    Entity m_Root;

};

} // namespace vrm
