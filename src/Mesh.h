#ifndef MESH_H
#define MESH_H

#include "Headers.h"
#include "Shader.h"
#include <string>
#include <vector>

class Mesh {
public:
	virtual void Draw(Shader shader) = 0;
protected:
	Mesh(std::vector<unsigned int> indices);
	Mesh();
	GLuint VAO;
	GLuint VBO, EBO;
	std::vector<unsigned int> indices;
	virtual void setupMesh() = 0;
};

#endif