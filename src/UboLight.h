#ifndef UBOLIGHT_H
#define UBOLIGHT_H

#include "Ubo.h"

class UboLight : public Ubo {
public:
	UboLight(glm::vec4 lightPosition, glm::vec4 lightColor);
	void inject(glm::vec4 lightPosition, glm::vec4 lightColor);
};

#endif