#include "MeshOrbit.h"

MeshOrbit::MeshOrbit(Shader shader, glm::vec3 color, float radius, int sideAmount)
	: radius(radius), sideAmount(sideAmount), MeshSolid(shader, color) {
	useLight = false;
	setupMesh();
}

void MeshOrbit::draw(Shader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setScale(scale);
	shader.setModel(world);
	shader.setUseLight(useLight);
	glBindVertexArray(VAO);
	glBindVertexBuffer(0, VBO, 0, sizeof(Vertex));
	glDrawArrays(GL_LINES, 0, sideAmount * 2);
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshOrbit::setRadius(float radius) {
	this->radius = radius;
	bufferData();
}

float MeshOrbit::getRadius() {
	return radius;
}

void MeshOrbit::setSideAmount(int sideAmount) {
	this->sideAmount = sideAmount;
	bufferData();
}

int MeshOrbit::getSideAmount() {
	return sideAmount;
}

void MeshOrbit::bufferData() {
	shader.use();
	glBindVertexArray(VAO);
	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
	}
	glGenBuffers(1, &VBO);

	if (sideAmount < 3) {
		printf("Orbit side amount cannot be lower than 3!");
		exit(1);
	}

	float angle = 0.0f;
	float radStep = 2 * M_PI / sideAmount;
	std::vector<Vertex> vertices;
	for (int i = 0; i < sideAmount; i++) {
		addSegment(&vertices, angle, i == sideAmount - 1 ? 0.0f : angle + radStep);
		angle += radStep;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);
}

void MeshOrbit::addSegment(std::vector<Vertex>* vertices, float angle1, float angle2) {
	Vertex first, second;
	first.Position.y = 0.0f;
	second.Position.y = 0.0f;
	first.Position.x = cos(angle1)*radius;
	first.Position.z = sin(angle1)*radius;
	second.Position.x = cos(angle2)*radius;
	second.Position.z = sin(angle2)*radius;
	first.Color = color;
	second.Color = color;
	vertices->push_back(first);
	vertices->push_back(second);
}

void MeshOrbit::setupMesh() {
	shader.use();
	glGenVertexArrays(1, &VAO);
	VBO = 0;
	bufferData();
}
