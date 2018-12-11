#include "UboLights.h"

UboLights::UboLights(int dirAmount, int pointAmount, int spotAmount, DirLight **dir, PointLight **point, SpotLight **spot)
	: Ubo(3 * sizeof(int) + MAX_LIGHTS_OF_TYPE * DIRLIGHT_SIZE + MAX_LIGHTS_OF_TYPE * POINTLIGHT_SIZE + MAX_LIGHTS_OF_TYPE * SPOTLIGHT_SIZE, "Lights", 3) {
	inject(dirAmount, pointAmount, spotAmount, dir, point, spot);
}

void UboLights::inject(int dirAmount, int pointAmount, int spotAmount, DirLight **dir, PointLight **point, SpotLight **spot) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	int dirs = glm::min(dirAmount, MAX_LIGHTS_OF_TYPE);
	int points = glm::min(pointAmount, MAX_LIGHTS_OF_TYPE);
	int spots = glm::min(spotAmount, MAX_LIGHTS_OF_TYPE);
	//void *ptr_v = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	//char *ptr = static_cast<char*>(ptr_v);
	//size_t offset = 0;
	//std::memcpy(ptr + offset, &dirs, sizeof(int));
	////glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &dirs);
	//offset += sizeof(int);
	//std::memcpy(ptr + offset, &points, sizeof(int));
	////glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &points);
	//offset += sizeof(int);
	//std::memcpy(ptr + offset, &spots, sizeof(int));
	////glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &spots);
	//offset += sizeof(int);
	//offset += sizeof(int); //padding
	//for (int i = 0; i < dirs; i++) {
	//	//glBufferSubData(GL_UNIFORM_BUFFER, offset, DIRLIGHT_SIZE, dir[i]);
	//	std::memcpy(ptr + offset, dir[i], DIRLIGHT_SIZE);
	//	offset += DIRLIGHT_SIZE;
	//}
	//if (dirs < MAX_LIGHTS_OF_TYPE) {
	//	offset += DIRLIGHT_SIZE * (MAX_LIGHTS_OF_TYPE - dirs);
	//}

	//for (int i = 0; i < points; i++) {
	//	//glBufferSubData(GL_UNIFORM_BUFFER, offset, POINTLIGHT_SIZE, point[i]);
	//	std::memcpy(ptr + offset, point[i], POINTLIGHT_SIZE);
	//	offset += POINTLIGHT_SIZE;
	//}
	//if (points < MAX_LIGHTS_OF_TYPE) {
	//	offset += POINTLIGHT_SIZE * (MAX_LIGHTS_OF_TYPE - points);
	//}

	//for (int i = 0; i < spots; i++) {
	//	//glBufferSubData(GL_UNIFORM_BUFFER, offset, SPOTLIGHT_SIZE, spot[i]);
	//	std::memcpy(ptr + offset, spot[i], SPOTLIGHT_SIZE);
	//	offset += SPOTLIGHT_SIZE;
	//}
	//glUnmapBuffer(GL_UNIFORM_BUFFER);

	size_t offset = 0;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &dirs);
	offset += sizeof(int);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &points);
	offset += sizeof(int);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &spots);
	offset += sizeof(int);
	offset += sizeof(int); //padding
	for (int i = 0; i < dirs; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, DIRLIGHT_SIZE, dir[i]);
		offset += DIRLIGHT_SIZE;
	}
	if (dirs < MAX_LIGHTS_OF_TYPE) {
		offset += DIRLIGHT_SIZE * (MAX_LIGHTS_OF_TYPE - dirs);
	}

	for (int i = 0; i < points; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, POINTLIGHT_SIZE, point[i]);
		offset += POINTLIGHT_SIZE;
	}
	if (points < MAX_LIGHTS_OF_TYPE) {
		offset += POINTLIGHT_SIZE * (MAX_LIGHTS_OF_TYPE - points);
	}

	for (int i = 0; i < spots; i++) {
		glBufferSubData(GL_UNIFORM_BUFFER, offset, SPOTLIGHT_SIZE, spot[i]);
		offset += SPOTLIGHT_SIZE;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
