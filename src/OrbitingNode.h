#ifndef ORBITINGNODE_H
#define ORBITINGNODE_H

#include "OrbitNode.h"

class OrbitingNode : public GraphNode {
public:
	OrbitingNode(OrbitNode *orbit, float orbitSpeed, Mesh *mesh = NULL);
	void update(double timeDiff);
	void draw() { GraphNode::draw(); };
	void setOrbitSpeed(float orbitSpeed);
private:
	float angle;
	float orbitSpeed;
	Mesh *mesh;
	OrbitNode *orbit;
};

#endif