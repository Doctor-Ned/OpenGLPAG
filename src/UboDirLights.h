#ifndef UBODIRLIGHTS_H
#define UBODIRLIGHTS_H

#include "Ubo.h"

class UboDirLights : public Ubo {
public:
	UboDirLights(int dirAmount, DirLight **dir);
	void inject(int dirAmount, DirLight **dir);
};

#endif