#include "MengerSponge.h"
#include <iterator>
#include <algorithm>

MengerSponge::MengerSponge() :
	MengerSponge(
		new glm::vec3(-HALF_SQRT_2, -HALF_SQRT_2, -HALF_SQRT_2),
		new glm::vec3(HALF_SQRT_2, HALF_SQRT_2, HALF_SQRT_2)) {}

MengerSponge::MengerSponge(glm::vec3* min, glm::vec3* max) {
	this->min = min;
	this->max = max;
	vao = new GLuint();
	vbo = new GLuint();
	glGenVertexArrays(1, vao);
	recreate(0);
}

void MengerSponge::render() {
	glBindVertexArray(*vao);
	glBindVertexBuffer(0, *vbo, 0, 8 * sizeof(float));
	glDrawArrays(GL_TRIANGLES, 0, vertexAmount);
	glBindVertexArray(0);
}

void MengerSponge::deleteBuffers() {
	glDeleteVertexArrays(1, vao);
	glDeleteBuffers(1, vbo);
	delete vao;
	delete vbo;
}

void MengerSponge::recreate(int recurseDepth) {
	if (*vbo != 0) {
		glDeleteBuffers(1, vbo);
	}
	glBindVertexArray(*vao);
	glGenBuffers(1, vbo);
	if (recurseDepth >= 0) {
		createVertices(recurseDepth, *min, *max);
		//createCube(*min, *max);
		vertexAmount = vertices.size() / 8;
		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)nullptr);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	vertices.clear();
}

GLuint* MengerSponge::getVAO() {
	return vao;
}

GLuint* MengerSponge::getVBO() {
	return vbo;
}

unsigned int MengerSponge::getVertexAmount() {
	return vertexAmount;
}

void MengerSponge::createCube(glm::vec3 min, glm::vec3 max) {
	float vertices[] = {
		//front
		min.x, min.y, max.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		max.x, min.y, max.z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		max.x, max.y, max.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		max.x, max.y, max.z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		min.x, max.y, max.z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		min.x, min.y, max.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		//back
		min.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		min.x, max.y, min.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		max.x, max.y, min.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		max.x, max.y, min.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		max.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		min.x, min.y, min.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		//left
		max.x, min.y, max.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		max.x, min.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		max.x, max.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		max.x, max.y, min.z, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		max.x, max.y, max.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		max.x, min.y, max.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		//right
		min.x, min.y, max.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		min.x, max.y, max.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		min.x, max.y, min.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		min.x, max.y, min.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		min.x, min.y, min.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		min.x, min.y, max.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		//top
		min.x, max.y, max.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		max.x, max.y, max.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		max.x, max.y, min.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		max.x, max.y, min.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		min.x, max.y, min.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		min.x, max.y, max.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		//bottom
		min.x, min.y, max.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		min.x, min.y, min.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		max.x, min.y, min.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		max.x, min.y, min.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		max.x, min.y, max.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		min.x, min.y, max.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	std::copy(&vertices[0], &vertices[sizeof(vertices) / sizeof(float)], std::back_inserter(this->vertices));
}

void MengerSponge::getCubeMinMax(glm::vec3& min, glm::vec3& max, glm::vec3 prevMin, glm::vec3 prevMax, int index) {
	getCubeMinMax(min, max, prevMin, prevMax, index % 3, (index % 9) / 3, index / 9);
}

void MengerSponge::getCubeMinMax(glm::vec3& min, glm::vec3& max, glm::vec3 prevMin, glm::vec3 prevMax, int col, int row,
                                 int level) {
	float xWidth = (prevMax.x - prevMin.x) / 3.0f,
	      yWidth = (prevMax.y - prevMin.y) / 3.0f,
	      zWidth = (prevMax.z - prevMin.z) / 3.0f;
	min.x = prevMin.x;
	min.y = prevMin.y;
	min.z = prevMin.z;
	max.x = min.x + xWidth;
	max.y = min.y + yWidth;
	max.z = min.z + zWidth;

	float addX = col * xWidth, addY = level * yWidth, addZ = row * zWidth;
	min.x += addX;
	max.x += addX;
	min.y += addY;
	max.y += addY;
	min.z += addZ;
	max.z += addZ;
}

void MengerSponge::createVertices(int recurseDepth, glm::vec3 min, glm::vec3 max) {
	static int current;
	static int indices[] = {
		0, 1, 2, 3, 5, 6, 7, 8, //bottom level
		9, 11, 15, 17, //middle level
		18, 19, 20, 21, 23, 24, 25, 26 //top level
	};
	if (recurseDepth == 0) {
		createCube(min, max);
	}
	else {
		recurseDepth--;
		for (int i = 0; i < 20; i++) {
			glm::vec3 currMin, currMax;
			getCubeMinMax(currMin, currMax, min, max, indices[i]);
			createVertices(recurseDepth, currMin, currMax);
		}
	}
}
