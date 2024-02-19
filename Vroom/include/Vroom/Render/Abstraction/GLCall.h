#pragma once

#include <GL/glew.h>

#include "Vroom/Core/Log.h"
#include "Vroom/Core/Assert.h"

#define GLCall(x) GLClearError();\
	x;\
	VRM_ASSERT(GLLogCall())

inline void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		LOG_WARN("[OpenGL Error] ({})", error);
		return false;
	}

	return true;
}