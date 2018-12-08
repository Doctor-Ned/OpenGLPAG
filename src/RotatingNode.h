#ifndef ROTATINGNODE_H
#define ROTATINGNODE_H

#include "GraphNode.h"

class RotatingNode : public GraphNode {
public:
	RotatingNode(float rotationSpeed, Mesh *mesh = NULL, GraphNode *parent = NULL);
	void update(double timeDiff);
	void draw() { GraphNode::draw(); };
	virtual void setRotationSpeed(float speed);
protected:
	float rotationSpeed;
};

#endif