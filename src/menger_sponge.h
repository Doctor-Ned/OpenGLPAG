#ifndef MENGER_SPONGE_H
#define MENGER_SPONGE_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
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

#define HALF_SQRT_2 sqrt(2)/4.0f

class menger_sponge {
public:
	menger_sponge();
	menger_sponge(glm::vec3 min, glm::vec3 max);
	void recreate(int recurseDepth);
	GLuint *getVAO();
	GLuint *getVBO();
	unsigned int getVertexAmount();
private:
	void createVertices(int recurseDepth, glm::vec3 min, glm::vec3 max);
	void createCube(glm::vec3 min, glm::vec3 max);
	void getCubeMinMax(glm::vec3 &min, glm::vec3 &max, glm::vec3 prevMin, glm::vec3 prevMax, int index);
	void getCubeMinMax(glm::vec3 &min, glm::vec3 &max, glm::vec3 prevMin, glm::vec3 prevMax, int row, int col, int level);
	glm::vec3 min, max;
	std::vector<float> vertices;
	GLuint *vao = NULL, *vbo = NULL;
	unsigned int vertexAmount;
	//std::vector<int> indices;
};

#endif