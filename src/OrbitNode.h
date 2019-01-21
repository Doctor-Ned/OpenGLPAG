#ifndef ORBITNODE_H
#define ORBITNODE_H

#include "GraphNode.h"

class OrbitNode : public GraphNode {
public:
	OrbitNode(float radius, Mesh* mesh = nullptr, GraphNode* parent = nullptr);
	void update(double timeDiff) override;
	float getRadius();
	void setRadius(float radius);
private:
	float radius;
};

#endif
