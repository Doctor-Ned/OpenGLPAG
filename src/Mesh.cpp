#include "Mesh.h"
void Mesh::draw(glm::mat4 world) {
	draw(shader, world);
}
void Mesh::draw(glm::mat4 world, float scale) {
	draw(shader, world, scale);
}

void Mesh::setShininess(float shininess) {
	this->shininess = shininess;
}

void Mesh::setUseLight(bool useLight) {
	this->useLight = useLight;
}
bool Mesh::getUseLight() {
	return useLight;
}

void Mesh::draw(Shader shader, glm::mat4 world, float scale) {
	shader.use();
	shader.setShininess(shininess);
	shader.setScale(scale);
	shader.setModel(world);
	shader.setUseLight(useLight);
	shader.setUseSpecular(false);
}
void Mesh::drawGui(bool autoUpdate) {}
void Mesh::setShader(Shader shader) {
	this->shader = shader;
}
Mesh::Mesh(Shader shader, std::vector<unsigned int> indices) : shader(shader), indices(indices) {}

Mesh::Mesh(Shader shader) : shader(shader) {}