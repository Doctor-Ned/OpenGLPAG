#ifndef MESHCYLINDERGEOMETRY_H
#define MESHCYLINDERGEOMETRY_H

#include "Headers.h"
#include "MeshSimple.h"
#include "GeometryShader.h"
#include <string>
#include <vector>

class MeshCylinderGeometry : public MeshSimple {
public:
	MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount, glm::vec3 baseCenter = glm::vec3(0.0f, 0.0f, 0.0f));
	void draw(glm::mat4 world, float scale = 1.0f);
	void draw(GeometryShader shader, glm::mat4 world, float scale = 1.0f);
	void draw(Shader shader, glm::mat4 world, float scale = 1.0f);
	void updateValues(float radius, float height, int sideAmount);
	glm::vec3 baseCenter;

protected:
	virtual void setupMesh();
	float height;
	float radius;
	int sideAmount;
	SimpleVertex vertex;
	GeometryShader shader;
};

#endif