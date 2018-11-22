#ifndef MESHSIMPLE_H
#define MESHSIMPLE_H

#include "Headers.h"
#include "Mesh.h"
#include <string>
#include <vector>

struct SimpleVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
};

class MeshSimple : public Mesh {
public:
	MeshSimple(Shader shader, std::vector<SimpleVertex> vertices, std::vector<unsigned int> indices);
	void draw(glm::mat4 world);
	void draw(Shader shader, glm::mat4 world);
protected:
	MeshSimple(Shader shader);
	void setupMesh();
	std::vector<SimpleVertex> vertices;
};

#endif