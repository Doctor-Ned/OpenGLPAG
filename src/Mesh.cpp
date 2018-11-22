#include "Mesh.h"
void Mesh::draw(glm::mat4 world) {
	draw(shader, world);
}
void Mesh::draw(glm::mat4 world, float scale) {
	shader.setScale(scale);
	draw(shader, world);
}
Mesh::Mesh(Shader shader, std::vector<unsigned int> indices) : shader(shader), indices(indices) {}

Mesh::Mesh(Shader shader) : shader(shader) {}