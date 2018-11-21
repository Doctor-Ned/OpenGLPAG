#include "UboLight.h"

UboLight::UboLight(glm::vec4 lightPosition, glm::vec4 lightColor) : Ubo(2*sizeof(glm::vec4), "Light", 1) {
	inject(lightPosition, lightColor);
}

void UboLight::inject(glm::vec4 lightPosition, glm::vec4 lightColor) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(lightPosition));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(lightColor));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
