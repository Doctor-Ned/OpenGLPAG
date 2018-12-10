#ifndef UBOLIGHTS_H
#define UBOLIGHTS_H

#include "Ubo.h"

#define MAX_LIGHTS_OF_TYPE 16          // this MUST be identical to the value from the shader

class UboLights : public Ubo {
public:
	UboLights(int dirAmount, int pointAmount, int spotAmount, DirLight **dir, PointLight **point, SpotLight **spot);
	void inject(int dirAmount, int pointAmount, int spotAmount, DirLight **dir, PointLight **point, SpotLight **spot);
};

#endif