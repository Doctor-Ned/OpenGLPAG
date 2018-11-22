#ifndef MESHCYLINDERGEOMETRY_H
#define MESHCYLINDERGEOMETRY_H

#include "Headers.h"
#include "MeshTexture.h"
#include "GeometryShader.h"
#include <string>
#include <vector>

class MeshCylinderGeometry : public MeshTexture {
public:
	MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount, char *texturePath, glm::vec3 baseCenter = glm::vec3(0.0f, 0.0f, 0.0f));
	void draw(glm::mat4 world, float scale = 1.0f);
	void draw(GeometryShader shader, glm::mat4 world, float scale = 1.0f);
	void draw(Shader shader, glm::mat4 world, float scale = 1.0f);
	void updateValues(float radius, float height, int sideAmount);
protected:
	glm::vec3 baseCenter;
	virtual void setupMesh();
	float height;
	float radius;
	int sideAmount;
	GeometryShader shader;
};

#endif