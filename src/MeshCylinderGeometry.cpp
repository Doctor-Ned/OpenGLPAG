#include "MeshCylinderGeometry.h"

MeshCylinderGeometry::MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount, char *texturePath, glm::vec3 baseCenter)
	: baseCenter(baseCenter), radius(radius), height(height), shader(shader), MeshTexture(shader) {
	texture = createTexture(texturePath);
	setupMesh();
}

void MeshCylinderGeometry::draw(glm::mat4 world, float scale) { draw(shader, world, scale); }

void MeshCylinderGeometry::draw(GeometryShader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setValues(radius, height, sideAmount);
	shader.setScale(scale);
	shader.setModel(world);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glBindVertexArray(VAO);
	glBindVertexBuffer(0, VBO, 0, sizeof(glm::vec3));
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshCylinderGeometry::draw(Shader shader, glm::mat4 world, float scale) {
	GeometryShader *geo = dynamic_cast<GeometryShader*>(&shader);
	if (geo != nullptr) {
		draw(*geo, world, scale);
	} else {
		printf("You tried to draw a MeshCylinderGeometry with a shader that is not a GeometryShader. That should not happen. Ever.\n");
		exit(1);
	}
}

void MeshCylinderGeometry::updateValues(float radius, float height, int sideAmount) {
	if (radius <= 0) {
		radius = 0.01f;
	}
	this->radius = radius;
	if (height <= 0) {
		height = 0.01f;
	}
	this->height = height;
	if (sideAmount < 3) {
		sideAmount = 3;
	}
	this->sideAmount = sideAmount;
}

void MeshCylinderGeometry::setupMesh() {
	shader.use();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &baseCenter, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
}
