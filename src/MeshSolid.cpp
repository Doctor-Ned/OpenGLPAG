#include "MeshSolid.h"

MeshSolid::MeshSolid(Shader shader, glm::vec3 color, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	: vertices(vertices), color(color), Mesh(shader, indices) {
	setupMesh();
}

void MeshSolid::draw(glm::mat4 world, float scale) { draw(shader, world, scale); }

MeshSolid::MeshSolid(Shader shader, glm::vec3 color) : color(color), Mesh(shader) {}

void MeshSolid::draw(Shader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setScale(scale);
	shader.setModel(world);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshSolid::setupMesh() {
	shader.use();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);
}
