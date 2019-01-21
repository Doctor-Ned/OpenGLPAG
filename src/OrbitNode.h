#ifndef ORBITNODE_H
#define ORBITNODE_H

#include "GraphNode.h"

class OrbitNode : public GraphNode {
public:
	OrbitNode(float radius, Mesh *mesh = NULL, GraphNode *parent = NULL);
	void update(double timeDiff);
	float getRadius();
	void setRadius(float radius);
private:
	float radius;
};

#endif