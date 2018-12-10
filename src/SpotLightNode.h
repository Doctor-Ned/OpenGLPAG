#ifndef SPOTLIGHTNODE_H
#define SPOTLIGHTNODE_H

#include "GraphNode.h"

class SpotLightNode : public GraphNode {
public:
	SpotLightNode(SpotLight *light, Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void update(double timeDiff);
	SpotLight *getLight();
protected:
	SpotLight *light;
};

#endif