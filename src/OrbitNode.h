#ifndef ORBITNODE_H
#define ORBITNODE_H

#include "GraphNode.h"
#include "MeshOrbit.h"

class OrbitNode : public GraphNode {
public:
	OrbitNode(MeshOrbit *mesh = NULL, GraphNode *parent = NULL);
	void update(double timeDiff);
	float getRadius();
	void setRadius(float radius);
	int getSideAmount();
	void setSideAmount(int sideAmount);
private:
	MeshOrbit *mesh;
	float radius;
	int sideAmount;
};

#endif