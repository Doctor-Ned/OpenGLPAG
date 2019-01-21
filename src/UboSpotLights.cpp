#include "UboSpotLights.h"

UboSpotLights::UboSpotLights(int spotAmount, SpotLight** spot)
	: Ubo(sizeof(int) + MAX_LIGHTS_OF_TYPE * SPOTLIGHT_SIZE, "SpotLights", 5) {
	inject(spotAmount, spot);
}

void UboSpotLights::inject(int spotAmount, SpotLight** spot) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	int spots = glm::min(spotAmount, MAX_LIGHTS_OF_TYPE);
	size_t offset = sizeof(glm::vec3);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &spots);
	offset += sizeof(int);
	for (int i = 0; i < spots; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, SPOTLIGHT_SIZE, spot[i]);
		offset += SPOTLIGHT_SIZE;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
