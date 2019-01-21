#ifndef UBOPOINTLIGHTS_H
#define UBOPOINTLIGHTS_H

#include "Ubo.h"

class UboPointLights : public Ubo {
public:
	UboPointLights(int pointAmount, PointLight** point);
	void inject(int pointAmount, PointLight** point);
};

#endif
