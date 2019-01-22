#include "OrbNode.h"

OrbNode::OrbNode(MeshSphere * mesh, float speed, glm::vec2 direction, GraphNode * parent) : GraphNode(mesh, parent) {
	radius = mesh->getRadius();
	this->speed = speed;
	this->direction = glm::normalize(direction);
}

OrbNode::OrbNode(MeshColorSphere * mesh, float speed, glm::vec2 direction, GraphNode * parent) : GraphNode(mesh, parent) {
	radius = mesh->getRadius();
	this->speed = speed;
	this->direction = glm::normalize(direction);
}

OrbNode::OrbNode(MeshRefSphere * mesh, float speed, glm::vec2 direction, GraphNode * parent) : GraphNode(mesh, parent) {
	radius = mesh->getRadius();
	this->speed = speed;
	this->direction = glm::normalize(direction);
}
void OrbNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
	setLocal(glm::translate(getLocal(), glm::vec3(direction*speed*(float)timeDiff, 0.0f)));
}

bool OrbNode::tryCollide(BlockNode* block) {
	CollisionState state = block->getCollisionState(this);
	if (state == None) {
		return false;
	}
	//block->onCollision();
	bounce(state);
	return true;
}

glm::vec2 OrbNode::getDirection() {
	return direction;
}

glm::vec3 OrbNode::actualCenter() {
	return getWorld()[3];
}

float OrbNode::getRadius() {
	return radius;
}

void OrbNode::bounce(CollisionState state) {
	switch (state) {
		case Top:
		case Bottom:
			direction.y = -direction.y;
			break;
		case Left:
		case Right:
			direction.x = -direction.x;
			break;
	}
	direction = glm::normalize(direction);
}
