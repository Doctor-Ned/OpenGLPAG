#ifndef MESHORBIT_H
#define MESHORBIT_H

#include "Headers.h"
#include "MeshSolid.h"
#include <string>
#include <vector>

class MeshOrbit : public MeshSolid {
public:
	MeshOrbit(Shader shader, glm::vec3 color, float radius, int sideAmount);
	void draw(glm::mat4 world, float scale = 1.0f);
	void draw(Shader shader, glm::mat4 world, float scale = 1.0f);
protected:
	void addSegment(std::vector<Vertex> *vertices, float angle1, float angle2);
	float radius;
	int sideAmount;
	void setupMesh();
};

#endif