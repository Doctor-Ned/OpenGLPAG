#include "Mesh.h"
void Mesh::draw(glm::mat4 world) {
	draw(shader, world);
}
void Mesh::draw(glm::mat4 world, float scale) {
	draw(shader, world, scale);
}
void Mesh::drawGui(bool autoUpdate) {}
void Mesh::setShader(Shader shader) {
	this->shader = shader;
}
Mesh::Mesh(Shader shader, std::vector<unsigned int> indices) : shader(shader), indices(indices) {}

Mesh::Mesh(Shader shader) : shader(shader) {}