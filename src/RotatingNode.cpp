#include "RotatingNode.h"

RotatingNode::RotatingNode(float rotationSpeed, Mesh * mesh, GraphNode * parent) : rotationSpeed(rotationSpeed), GraphNode(mesh, parent) {}

void RotatingNode::update(double timeDiff) {
	setLocal(glm::rotate(local, (float)(rotationSpeed*timeDiff), glm::vec3(0.0f, 1.0f, 0.0f)));
	GraphNode::update(timeDiff);
}

void RotatingNode::setRotationSpeed(float speed) {
	this->rotationSpeed = speed;
}
