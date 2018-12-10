#include "UboLights.h"

UboLights::UboLights(int dirAmount, int pointAmount, int spotAmount, DirLight *dir, PointLight *point, SpotLight *spot)
	: Ubo(3 * sizeof(int) + MAX_LIGHTS_OF_TYPE * sizeof(DirLight) + MAX_LIGHTS_OF_TYPE * sizeof(PointLight) + MAX_LIGHTS_OF_TYPE * sizeof(SpotLight), "Lights", 3) {
	inject(dirAmount, pointAmount, spotAmount, dir, point, spot);
}

void UboLights::inject(int dirAmount, int pointAmount, int spotAmount, DirLight *dir, PointLight *point, SpotLight *spot) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	int dirs = glm::min(dirAmount, MAX_LIGHTS_OF_TYPE);
	int points = glm::min(pointAmount, MAX_LIGHTS_OF_TYPE);
	int spots = glm::min(spotAmount, MAX_LIGHTS_OF_TYPE);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &dirs);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int), sizeof(int), &points);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int) * 2, sizeof(int), &spots);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int) * 3, sizeof(DirLight) * dirs, dir);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int) * 3 + sizeof(DirLight) * MAX_LIGHTS_OF_TYPE, sizeof(PointLight) * points, point);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(int) * 3 + sizeof(DirLight) * MAX_LIGHTS_OF_TYPE + sizeof(PointLight) * MAX_LIGHTS_OF_TYPE, sizeof(SpotLight) * spots, spot);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
