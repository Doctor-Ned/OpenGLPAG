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
	MeshSimple(std::vector<SimpleVertex> vertices, std::vector<unsigned int> indices);
	virtual void Draw(Shader shader);
protected:
	MeshSimple();
	virtual void setupMesh();
	std::vector<SimpleVertex> vertices;
};

#endif