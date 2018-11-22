#ifndef ROTATINGNODE_H
#define ROTATINGNODE_H

#include "GraphNode.h"

class RotatingNode : public GraphNode {
public:
	RotatingNode(float rotationSpeed, Mesh *mesh = NULL, GraphNode *parent = NULL);
	void update(float timeDiff);
	void draw() { GraphNode::draw(); };
	virtual void setRotationSpeed(float speed);
	glm::mat4 getLocal() { return GraphNode::getLocal(); };
	glm::mat4 getWorld() { return GraphNode::getWorld(); };
	void setScale(float scale) { GraphNode::setScale(scale); };
	void setLocal(glm::mat4 local) { GraphNode::setLocal(local); };
	void addChild(GraphNode *child) { GraphNode::addChild(child); };
	void removeChild(GraphNode *child) { GraphNode::removeChild(child); };
protected:
	float rotationSpeed;
};

#endif