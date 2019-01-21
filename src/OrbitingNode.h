#ifndef ORBITINGNODE_H
#define ORBITINGNODE_H

#include "OrbitNode.h"

class OrbitingNode : public GraphNode {
public:
	OrbitingNode(OrbitNode* orbit, float orbitSpeed, Mesh* mesh = nullptr);
	void update(double timeDiff) override;
	void draw() { GraphNode::draw(); };
	void setOrbitSpeed(float orbitSpeed);
	void setAngle(float angle);
	float getAngle();
private:
	float angle;
	float orbitSpeed;
	Mesh* mesh;
	OrbitNode* orbit;
};

#endif
