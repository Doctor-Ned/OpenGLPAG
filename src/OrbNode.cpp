#include "OrbNode.h"

OrbNode::OrbNode(MeshSphere * mesh, float speed, glm::vec2 direction, GraphNode * parent) {
	radius = mesh->getRadius();
	this->speed = speed;
	this->direction = direction;
}

void OrbNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
	//TODO: add proper movement and collisions
}
