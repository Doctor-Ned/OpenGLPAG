#include "MeshOrbit.h"

MeshOrbit::MeshOrbit(Shader shader, glm::vec3 color, float radius, int sideAmount)
	: radius(radius), sideAmount(sideAmount), MeshSolid(shader, color) {
	setupMesh();
}

void MeshOrbit::draw(glm::mat4 world, float scale) { draw(shader, world, scale); }

void MeshOrbit::draw(Shader shader, glm::mat4 world, float scale) {
	//shader.use();
	shader.setScale(scale);
	shader.setModel(world);
	glBindVertexArray(VAO);
	glBindVertexBuffer(0, VBO, 0, sizeof(Vertex));
	glDrawArrays(GL_LINES, 0, sideAmount * 2);
	glBindVertexArray(0);
}

void MeshOrbit::addSegment(std::vector<Vertex>* vertices, float angle1, float angle2) {
	Vertex first, second;
	first.Position.y = 0.0f;
	second.Position.y = 0.0f;
	first.Position.x = cos(angle1);
	first.Position.z = sin(angle1);
	second.Position.x = cos(angle2);
	second.Position.z = sin(angle2);
	first.Color = color;
	second.Color = color;
	vertices->push_back(first);
	vertices->push_back(second);
}

void MeshOrbit::setupMesh() {
	glGenVertexArrays(1, &VAO);
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

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);
}
