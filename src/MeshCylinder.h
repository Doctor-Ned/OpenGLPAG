#ifndef MESHCYLINDER_H
#define MESHCYLINDER_H

#include "Headers.h"
#include "MeshTexture.h"
#include <string>
#include <vector>

class MeshCylinder : public MeshTexture {
public:
	MeshCylinder(float radius, float height, int sideAmount, char *texturePath, glm::vec3 baseCenter = glm::vec3(0.0f, -0.5f, 0.0f));
	virtual void Draw(Shader shader);
	void updateValues(float radius, float height, int sideAmount);
	glm::vec3 baseCenter;
protected:
	void createBottomTriangle(std::vector<TextureVertex> *vertices, float angle1, float angle2);
	void createTopTriangle(std::vector<TextureVertex> *vertices);
	void createSideTriangles(std::vector<TextureVertex> *vertices);
	void bufferData(std::vector<TextureVertex> *vertices);
	virtual void setupMesh();
	float height;
	float radius;
	int sideAmount;
	unsigned int vertexAmount;
};

#endif