#ifndef MESHCYLINDERGEOMETRY_H
#define MESHCYLINDERGEOMETRY_H

#include "Headers.h"
#include "MeshSimple.h"
#include "GeometryShader.h"
#include <string>
#include <vector>

class MeshCylinderGeometry : public MeshSimple {
public:
	MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount);
	void draw(glm::mat4 world);
	void draw(GeometryShader shader, glm::mat4 world);
	void draw(Shader shader, glm::mat4 world);
	void updateValues(float radius, float height, int sideAmount);
protected:
	virtual void setupMesh();
	float height;
	float radius;
	int sideAmount;
	SimpleVertex vertex;
	GLint uniHeight, uniRadius, uniSide;
};

#endif