#include "OrbNode.h"

OrbNode::OrbNode(MeshSphere * mesh, float speed, glm::vec2 direction, GraphNode * parent) {
	radius = mesh->getRadius();
	this->speed = speed;
	this->direction = glm::normalize(direction);
}

void OrbNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
	//TODO: add proper movement and collisions
}

bool OrbNode::tryCollide(BlockNode* block) {
	CollisionState state = block->getCollisionState(this);
	if(state == None) {
		return false;
	}
	block->onCollision();
	bounce(state);
	return true;
}

float OrbNode::getRadius() {
	return radius;
}

void OrbNode::bounce(CollisionState state) {
	switch(state) {
		case Top:
		case Bottom:
			direction.y = -direction.y;
			break;
		case Left:
		case Right:
			direction.x = -direction.x;
			break;
	}
}
