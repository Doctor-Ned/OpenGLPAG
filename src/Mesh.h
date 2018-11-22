#ifndef MESH_H
#define MESH_H

#include "Headers.h"
#include "Shader.h"
#include <string>
#include <vector>

class Mesh {
public:
	virtual void draw(glm::mat4 world);
	virtual void draw(glm::mat4 world, float scale);
	virtual void draw(Shader shader, glm::mat4 world, float scale = 1.0f) = 0;
protected:
	Mesh(Shader shader, std::vector<unsigned int> indices);
	Mesh(Shader shader);
	GLuint VAO;
	GLuint VBO, EBO;
	std::vector<unsigned int> indices;
	Shader shader;
	virtual void setupMesh() = 0;
};

#endif