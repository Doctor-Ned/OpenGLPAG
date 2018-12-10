#ifndef POINTLIGHTNODE_H
#define POINTLIGHTNODE_H

#include "GraphNode.h"

class PointLightNode : public GraphNode {
public:
	PointLightNode(PointLight *light, Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void update(double timeDiff);
	PointLight *getLight();
protected:
	PointLight *light;
};

#endif