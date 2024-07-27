#pragma once

#include <GL/glew.h>

#include "Vroom/Core/Log.h"
#include "Vroom/Core/Assert.h"

#ifdef VRM_DEBUG
	/** 
	 * To be wrapped around an OpenGL function. Will display errors in debug mode.
	 */
	#define GLCall(x) GLClearError();\
		x;\
		VRM_ASSERT(GLLogCall())

	#define GLCall_nothrow(x) GLClearError();\
		x;\
		GLLogCall()
#else
	/** 
	 * To be wrapped around an OpenGL function. Will display errors in debug mode.
	 */
	#define GLCall(x) x
	#define GLCall_nothrow(x) x
#endif

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
		LOG_WARN("[OpenGL Error] (0x{:04x})", error);
		return false;
	}

	return true;
}