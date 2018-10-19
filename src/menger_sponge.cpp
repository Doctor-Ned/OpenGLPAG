#include "menger_sponge.h"
#include <iterator>
#include <algorithm>

menger_sponge::menger_sponge() :
	menger_sponge::menger_sponge(
		glm::vec3(-HALF_SQRT_2, -HALF_SQRT_2, -HALF_SQRT_2),
		glm::vec3(HALF_SQRT_2, HALF_SQRT_2, HALF_SQRT_2)) {}

menger_sponge::menger_sponge(glm::vec3 min, glm::vec3 max) {
	this->min = min;
	this->max = max;
	vao = new GLuint();
	vbo = new GLuint();
	glCreateVertexArrays(1, vao);
	recreate(0);
}

void menger_sponge::recreate(int recurseDepth) {
	glBindVertexArray(*vao);
	if (*vbo != 0) {
		glDeleteBuffers(1, vbo);
	}
	glGenBuffers(1, vbo);
	if (recurseDepth >= 0) {
		createVertices(recurseDepth, min, max);
		vertexAmount = vertices.size();
		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexAmount * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)3);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)6);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
	vertices.clear();
}

GLuint * menger_sponge::getVAO() {
	return vao;
}

GLuint * menger_sponge::getVBO() {
	return vbo;
}

unsigned int menger_sponge::getVertexAmount() {
	return vertexAmount;
}

void menger_sponge::createCube(glm::vec3 min, glm::vec3 max) {
	float vertices[] = {
		//front
		min.x,min.y,max.z,0.0f,0.0f,1.0f,0.0f,0.0f,
		max.x,min.y,max.z,0.0f,0.0f,1.0f,1.0f,0.0f,
		max.x,max.y,max.z,0.0f,0.0f,1.0f,1.0f,1.0f,
		min.x,max.y,max.z,0.0f,0.0f,1.0f,0.0f,1.0f,
		//back
		min.x,min.y,min.z,0.0f,0.0f,-1.0f,0.0f,0.0f,
		min.x,max.y,min.z,0.0f,0.0f,-1.0f,1.0f,0.0f,
		max.x,max.y,min.z,0.0f,0.0f,-1.0f,1.0f,1.0f,
		max.x,min.y,min.z,0.0f,0.0f,-1.0f,0.0f,1.0f,
		//left
		max.x,min.y,max.z,1.0f,0.0f,0.0f,0.0f,0.0f,
		max.x,min.y,min.z,1.0f,0.0f,0.0f,1.0f,0.0f,
		max.x,max.y,min.z,1.0f,0.0f,0.0f,1.0f,1.0f,
		max.x,max.y,max.z,1.0f,0.0f,0.0f,0.0f,1.0f,
		//right
		min.x,min.y,max.z,-1.0f,0.0f,0.0f,0.0f,0.0f,
		min.x,max.y,max.z,-1.0f,0.0f,0.0f,1.0f,0.0f,
		min.x,max.y,min.z,-1.0f,0.0f,0.0f,1.0f,1.0f,
		min.x,min.y,min.z,-1.0f,0.0f,0.0f,0.0f,1.0f,
		//top
		min.x,max.y,max.z,0.0f,1.0f,0.0f,0.0f,0.0f,
		max.x,max.y,max.z,0.0f,1.0f,0.0f,1.0f,0.0f,
		max.x,max.y,min.z,0.0f,1.0f,0.0f,1.0f,1.0f,
		min.x,max.y,min.z,0.0f,1.0f,0.0f,0.0f,1.0f,
		//bottom
		min.x,min.y,max.z,0.0f,-1.0f,0.0f,0.0f,0.0f,
		min.x,min.y,min.z,0.0f,-1.0f,0.0f,1.0f,0.0f,
		max.x,min.y,min.z,0.0f,-1.0f,0.0f,1.0f,1.0f,
		max.x,min.y,max.z,0.0f,-1.0f,0.0f,0.0f,1.0f
	};
	//int indices[] = {
	//	indOffset + 0,indOffset + 1,indOffset + 2,
	//	indOffset + 2,indOffset + 3,indOffset + 0,

	//	indOffset + 4,indOffset + 5,indOffset + 6,
	//	indOffset + 6,indOffset + 7,indOffset + 4,

	//	indOffset + 8,indOffset + 9,indOffset + 10,
	//	indOffset + 10,indOffset + 11,indOffset + 8,

	//	indOffset + 12,indOffset + 13,indOffset + 14,
	//	indOffset + 14,indOffset + 15,indOffset + 12,

	//	indOffset + 16,indOffset + 17,indOffset + 18,
	//	indOffset + 18,indOffset + 19,indOffset + 16,

	//	indOffset + 20,indOffset + 21,indOffset + 22,
	//	indOffset + 22,indOffset + 23,indOffset + 20
	//};

	std::copy(&vertices[0], &vertices[192], std::back_inserter(this->vertices));  // 192 = 8*4*6
}

void menger_sponge::getCubeMinMax(glm::vec3 &min, glm::vec3 &max, glm::vec3 prevMin, glm::vec3 prevMax, int index) {
	getCubeMinMax(min, max, prevMin, prevMax, index % 3, (index % 9) / 3, index / 9);
}

void menger_sponge::getCubeMinMax(glm::vec3 & min, glm::vec3 & max, glm::vec3 prevMin, glm::vec3 prevMax, int col, int row, int level) {
	float xWidth = (prevMax.x - prevMin.x) / 3.0f,
		yWidth = (prevMax.y - prevMin.y) / 3.0f,
		zWidth = (prevMax.z - prevMin.z) / 3.0f;
	min.x = prevMin.x;
	min.y = prevMin.y;
	min.z = prevMin.z;
	max.x = min.x + xWidth;
	max.y = min.y + yWidth;
	max.z = min.z + zWidth;

	float addX = 0, addY = 0, addZ = 0;

}

void menger_sponge::createVertices(int recurseDepth, glm::vec3 min, glm::vec3 max) {
	static int current;
	static int indices[] = {
		0,1,2,3,4,5,6,7,8,         //bottom level
		9,11,15,17,                //middle level
		18,19,20,21,22,23,24,25,26 //top level
	};
	if (recurseDepth == 0) {
		createCube(min, max);
	} else {
		recurseDepth--;
		for (int i = 0; i < 20; i++) {
			glm::vec3 currMin, currMax;
			getCubeMinMax(currMin, currMax, min, max, indices[i]);
			createVertices(recurseDepth, currMin, currMax);
		}
	}
}

