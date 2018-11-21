#ifndef UTIL_H
#define UTIL_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#define UBO_LIGHT "Light"
#define UBO_VIEWPROJECTION "ViewProjection"

class Util {
public:
	static GLuint createAndCompileShader(int shaderType, const char* file);
	static void linkShaderProgram(GLuint shaderProgram);
	static void bindUniformBlock(GLuint shaderProgram, const char* blockName, GLuint binding);
	static GLuint createUboViewProjection(GLuint binding);
	static void injectUboViewProjection(GLuint ubo, glm::mat4 view, glm::mat4 projection);
	static GLuint createUboLight(GLuint binding);
	static void injectUboLight(GLuint ubo, glm::vec4 lightPosition, glm::vec4 lightColor);
private:
	static GLuint createUbo(unsigned int size, GLuint binding);
};

#endif