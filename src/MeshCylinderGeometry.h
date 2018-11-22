#ifndef MESHCYLINDERGEOMETRY_H
#define MESHCYLINDERGEOMETRY_H

#include "Headers.h"
#include "MeshSimple.h"
#include "GeometryShader.h"
#include <string>
#include <vector>

class MeshCylinderGeometry : public MeshSimple {
public:
	MeshCylinderGeometry(float radius, float height, int sideAmount);
	virtual void Draw(GeometryShader shader);
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