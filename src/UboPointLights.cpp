#include "UboPointLights.h"

UboPointLights::UboPointLights(int pointAmount, PointLight **point)
	: Ubo(sizeof(int) + MAX_LIGHTS_OF_TYPE * POINTLIGHT_SIZE, "PointLights", 4) {
	inject(pointAmount, point);
}

void UboPointLights::inject(int pointAmount, PointLight **point) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	int points = glm::min(pointAmount, MAX_LIGHTS_OF_TYPE);
	size_t offset = sizeof(glm::vec3);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &points);
	offset += sizeof(int);
	for (int i = 0; i < points; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, POINTLIGHT_SIZE, point[i]);
		offset += POINTLIGHT_SIZE;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
