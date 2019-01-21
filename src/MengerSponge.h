#ifndef MENGERSPONGE_H
#define MENGERSPONGE_H

#include "Headers.h"
#define HALF_SQRT_2 sqrt(2)/4.0f

class MengerSponge {
public:
	MengerSponge();
	MengerSponge(glm::vec3* min, glm::vec3* max);
	void render();
	void deleteBuffers();
	void recreate(int recurseDepth);
	GLuint* getVAO();
	GLuint* getVBO();
	unsigned int getVertexAmount();
private:
	void createVertices(int recurseDepth, glm::vec3 min, glm::vec3 max);
	void createCube(glm::vec3 min, glm::vec3 max);
	void getCubeMinMax(glm::vec3& min, glm::vec3& max, glm::vec3 prevMin, glm::vec3 prevMax, int index);
	void getCubeMinMax(glm::vec3& min, glm::vec3& max, glm::vec3 prevMin, glm::vec3 prevMax, int row, int col,
	                   int level);
	glm::vec3 *min, *max;
	std::vector<float> vertices;
	GLuint *vao = nullptr, *vbo = nullptr;
	unsigned int vertexAmount;
	//std::vector<int> indices;
};

#endif
