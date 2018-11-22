#ifndef MESHSOLID_H
#define MESHSOLID_H

#include "Headers.h"
#include "Mesh.h"
#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Color;
};

class MeshSolid : public Mesh {
public:
	MeshSolid(Shader shader, glm::vec3 color, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void draw(glm::mat4 world, float scale = 1.0f);
	void draw(Shader shader, glm::mat4 world, float scale = 1.0f);
protected:
	MeshSolid(Shader shader, glm::vec3 color);
	void setupMesh();
	std::vector<Vertex> vertices;
	glm::vec3 color;
};

#endif