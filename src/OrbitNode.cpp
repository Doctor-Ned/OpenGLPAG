#include "OrbitNode.h"

OrbitNode::OrbitNode(float radius, Mesh* mesh, GraphNode* parent) : GraphNode(mesh, parent), radius(radius) { }

void OrbitNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
}

float OrbitNode::getRadius() {
	return radius;
}

void OrbitNode::setRadius(float radius) {
	if (radius > 0.0f) {
		this->radius = radius;
	}
}
