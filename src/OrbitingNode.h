#ifndef ORBITINGNODE_H
#define ORBITINGNODE_H

#include "OrbitNode.h"

class OrbitingNode : public GraphNode {
public:
	OrbitingNode(OrbitNode *orbit, float orbitSpeed, Mesh *mesh = NULL);
	void update(double timeDiff);
	void draw() { GraphNode::draw(); };
	void setOrbitSpeed(float orbitSpeed);
	glm::mat4 getLocal() { return GraphNode::getLocal(); };
	glm::mat4 getWorld() { return GraphNode::getWorld(); };
	void setScale(float scale) { GraphNode::setScale(scale); };
	void setLocal(glm::mat4 local) { GraphNode::setLocal(local); };
	void addChild(GraphNode *child) { GraphNode::addChild(child); };
	void removeChild(GraphNode *child) { GraphNode::removeChild(child); };
private:
	float angle;
	float orbitSpeed;
	Mesh *mesh;
	OrbitNode *orbit;
};

#endif