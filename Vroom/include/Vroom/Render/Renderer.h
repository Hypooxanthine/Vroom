#pragma once

#include <glm/glm.hpp>

#include "Vroom/Render/Abstraction/GLCall.h"
#include "Vroom/Render/Abstraction/VertexArray.h"
#include "Vroom/Render/Abstraction/VertexBuffer.h"
#include "Vroom/Render/Abstraction/VertexBufferLayout.h"
#include "Vroom/Render/Abstraction/IndexBuffer.h"
#include "Vroom/Render/Abstraction/Shader.h"

/**
 * @brief Draws things with OpenGL.
 */
class Renderer
{
public:
	Renderer();
	~Renderer();

	/**
	 * @brief Has to be called before any rendering of current frame.
	 * 
	 */
	void beginScene();

	/**
	 * @brief Has to be called after any rendering scene.
	 * @warning Everything sent to draw functions need to be still alive when calling endScene.
	 * 
	 */
	void endScene();

	/**
	 * @brief Performs a draw call of triangles with OpenGL using a @ref VertexArray, an @ref IndexBuffer and a @ref Shader.
	 * 
	 * @param va VertexArray instance.
	 * @param ib IndexBuffer instance.
	 * @param shader Shader instance.
	 */
	void drawTriangles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	/**
	 * @brief Performs a draw call of points with OpenGL using a @ref VertexArray, an @ref IndexBuffer and a @ref Shader.
	 * 
	 * @param va 
	 * @param shader 
	 */
	void drawPoints(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	const glm::vec<2, unsigned int>& getViewportSize() const;

	void setViewport(const glm::vec<2, unsigned int>& o, const glm::vec<2, unsigned int>& s);
	void setViewportOrigin(const glm::vec<2, unsigned int>& o);
	void setViewportSize(const glm::vec<2, unsigned int>& s);

private:
	glm::vec<2, unsigned int> m_ViewportOrigin = { 0, 0 };
	glm::vec<2, unsigned int> m_ViewportSize = { 0, 0 };
};