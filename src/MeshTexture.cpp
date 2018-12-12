#include "MeshTexture.h"

MeshTexture::MeshTexture(Shader shader, std::vector<TextureVertex> vertices, std::vector<unsigned int> indices, char * textureFile)
	: vertices(vertices), Mesh(shader, indices) {
	this->texture = createTexture(textureFile);
	setupMesh();
}

void MeshTexture::draw(Shader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setScale(scale);
	shader.setModel(world);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

MeshTexture::MeshTexture(Shader shader) : Mesh(shader) {}

void MeshTexture::setupMesh() {
	shader.use();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TextureVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (void*)offsetof(TextureVertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (void*)offsetof(TextureVertex, TexCoords));

	glBindVertexArray(0);
}
