#include "OrbitNode.h"

OrbitNode::OrbitNode(MeshOrbit *mesh, GraphNode *parent) : mesh(mesh), GraphNode(mesh, parent) {
	if (mesh != NULL) {
		radius = mesh->getRadius();
		sideAmount = mesh->getSideAmount();
	} else {
		radius = 1.0f;
		sideAmount = 3;
	}
}

void OrbitNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
}

float OrbitNode::getRadius() {
	return radius;
}

void OrbitNode::setRadius(float radius) {
	if (radius > 0.0f) {
		this->radius = radius;
		if (mesh != NULL) {
			mesh->setRadius(radius);
		}
	}
}

int OrbitNode::getSideAmount() {
	return sideAmount;
}

void OrbitNode::setSideAmount(int sideAmount) {
	if (sideAmount >= 3) {
		this->sideAmount = sideAmount;
		if (mesh != NULL) {
			mesh->setSideAmount(sideAmount);
		}
	}
}
