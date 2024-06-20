#pragma once

#include <GL/glew.h>

#include "Vroom/Core/Log.h"
#include "Vroom/Core/Assert.h"

/** 
 * To be wrapped around an OpenGL function. Will display errors in debug mode.
 * TODO: handle debug/release modes.
 */
#define GLCall(x) GLClearError();\
	x;\
	VRM_ASSERT(GLLogCall())

#define GLCall_nothrow(x) GLClearError();\
	x;\
	GLLogCall()

/**
 * @brief Clears OpenGL errors queue.
 */
inline void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

/**
 * @brief Displays all OpenGL errors.
 * @return True if no error. False if errors.
 */
inline bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		LOG_WARN("[OpenGL Error] ({})", error);
		return false;
	}

	return true;
}