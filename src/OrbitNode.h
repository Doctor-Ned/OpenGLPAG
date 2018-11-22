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
	glm::mat4 getLocal() { return GraphNode::getLocal(); };
	glm::mat4 getWorld() { return GraphNode::getWorld(); };
	void setScale(float scale) { GraphNode::setScale(scale); };
	void setLocal(glm::mat4 local) { GraphNode::setLocal(local); };
	void addChild(GraphNode *child) { GraphNode::addChild(child); };
	void removeChild(GraphNode *child) { GraphNode::removeChild(child); };
private:
	MeshOrbit *mesh;
	float radius;
	int sideAmount;
};

#endif