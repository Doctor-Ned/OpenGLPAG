#include "UboDirLights.h"

UboDirLights::UboDirLights(int dirAmount, DirLight** dir)
	: Ubo(sizeof(int) + MAX_LIGHTS_OF_TYPE * DIRLIGHT_SIZE, "DirLights", 3) {
	inject(dirAmount, dir);
}

void UboDirLights::inject(int dirAmount, DirLight** dir) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	int dirs = glm::min(dirAmount, MAX_LIGHTS_OF_TYPE);
	size_t offset = sizeof(glm::vec3);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &dirs);
	offset += sizeof(int);
	for (int i = 0; i < dirs; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, DIRLIGHT_SIZE, dir[i]);
		offset += DIRLIGHT_SIZE;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
