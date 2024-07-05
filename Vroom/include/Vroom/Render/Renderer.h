#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Vroom/Render/Clustering/LightRegistry.h"

#include "Vroom/Asset/AssetInstance/MeshInstance.h"

#include "Vroom/Render/Camera/CameraBasic.h"

namespace vrm
{

class Scene;
struct PointLightComponent;

/**
 * @brief The renderer is responsible for rendering objects on the scene, taking lights and cameras into consideration.
 * 
 * @todo Point lights data. Currently, point lights radius and intensity are sent to the shaders but are not used. It only needs to be implemented in the shading model shader.
 * 
 * @todo  Point lights SSBO optimization. We should probably save the data of lights for the next frame, because if no light is added/removed,
 * we don't need to recreate the SSBO from scratch. And if some light states were only changed, we could only update the SSBO with the new data.
 * 
 * @todo Forward+ rendering. Currently arbitrary number of lights are supported, but the performance is not the best because each object is taking all lights into consideration. Forward+ rendering would help with this be dividing the view space into clusters, and only taking into consideration the lights that affect the cluster. Then, when rendering an object from a cluster, only the lights that affect that cluster are taken into consideration.
 */
class Renderer
{
public:
	/**
	 * @brief Initializes OpenGl settings for the engine.
	 */
	Renderer();

	/**
	 * @brief Releases GPU memory.
	 */
	~Renderer();

	/**
	 * @brief Has to be called before any rendering of current frame.
	 * 
	 */
	void beginScene(const Scene& scene);

	/**
	 * @brief Has to be called after any rendering scene.
	 * @warning Everything sent to draw functions need to be still alive when calling endScene.
	 * 
	 */
	void endScene();

	/**
	 * @brief Submits a mesh to be drawn.
	 * 
	 * @warning Model matrix must be still alive when calling endScene. This is needed because the renderer does not store the data, it only stores references to it.
	 * No worries about the mesh, the mesh instance guarantees that the mesh is still alive.
	 * 
	 * @param mesh  The mesh to submit.
	 * @param model  The model matrix.
	 */
	void submitMesh(const MeshInstance& mesh, const glm::mat4& model);

	/**
	 * @brief Submits a point light to be drawn.
	 * 
	 * @warning Position and pointLightComponent must be still alive when calling endScene. This is needed because the renderer does not store the data, it only stores references to it.
	 * 
	 * @param position  The position of the light.
	 * @param pointLight  The point light component.
	 * @param identifier  The identifier of the light.
	 */
	void submitPointLight(const glm::vec3& position, const PointLightComponent& pointLight, const std::string& identifier);

	/**
	 * @brief  Draws a mesh with a shader and a camera.
	 * 
	 * @param mesh  The mesh to draw.
	 * @param model  The model matrix.
	 */
	void drawMesh(const MeshInstance& mesh, const glm::mat4& model) const;

	/**
	 * @brief Gets the viewport origin.
	 * @return The viewport origin.
	 */
	const glm::vec<2, unsigned int>& getViewportOrigin() const;

	/**
	 * @brief Gets the viewport size.
	 * @return The viewport size.
	 */
	const glm::vec<2, unsigned int>& getViewportSize() const;

	/**
	 * @brief Sets viewport settings.
	 * @param o New viewport origin.
	 * @param s New viewport size.
	 */
	void setViewport(const glm::vec<2, unsigned int>& o, const glm::vec<2, unsigned int>& s);

	/**
	 * @brief Sets viewport origin.
	 * @param o New viewport origin.
	 */
	void setViewportOrigin(const glm::vec<2, unsigned int>& o);

	/**
	 * @brief Sets viewport size.
	 * @param s New viewport size.
	 */
	void setViewportSize(const glm::vec<2, unsigned int>& s);

private:
	// Structs to store data to be drawn
	struct QueuedMesh
	{
		MeshInstance mesh;
		const glm::mat4& model;
	};

private:
	glm::vec<2, unsigned int> m_ViewportOrigin = { 0, 0 };
	glm::vec<2, unsigned int> m_ViewportSize = { 0, 0 };

	const CameraBasic* m_Camera = nullptr;

	std::vector<QueuedMesh> m_Meshes;

	LightRegistry m_LightRegistry;
};

} // namespace vrm