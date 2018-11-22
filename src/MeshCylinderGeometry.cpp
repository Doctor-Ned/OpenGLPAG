#include "MeshCylinderGeometry.h"

MeshCylinderGeometry::MeshCylinderGeometry(GeometryShader shader, float radius, float height, int sideAmount, glm::vec3 baseCenter)
	: baseCenter(baseCenter), radius(radius), height(height), shader(shader), MeshSimple(shader) {
	vertex.Position.x = 0.0f;
	vertex.Position.y = 0.0f;
	vertex.Position.z = 0.0f;
	setupMesh();
}

void MeshCylinderGeometry::draw(glm::mat4 world, float scale) { draw(shader, world, scale); }

void MeshCylinderGeometry::draw(GeometryShader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setValues(radius, height, sideAmount);
	shader.setScale(scale);
	shader.setModel(world);
	glBindVertexArray(VAO);
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
	this->radius = radius;
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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &vertex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
}
