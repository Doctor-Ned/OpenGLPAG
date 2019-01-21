#ifndef UBOSPOTLIGHTS_H
#define UBOSPOTLIGHTS_H

#include "Ubo.h"

class UboSpotLights : public Ubo {
public:
	UboSpotLights(int spotAmount, SpotLight** spot);
	void inject(int spotAmount, SpotLight** spot);
};

#endif
