#include "BlockNode.h"
#include "OrbNode.h"

BlockNode::BlockNode(MeshBox * mesh, GraphNode * parent) : GraphNode(mesh, parent) {
	this->min = mesh->getMin();
	this->max = mesh->getMax();
}

BlockNode::BlockNode(MeshColorBox * mesh, GraphNode * parent) : GraphNode(mesh, parent) {
	this->min = mesh->getMin();
	this->max = mesh->getMax();
}

void BlockNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
}

void BlockNode::onCollision(GameScene *gameScene) {

}

CollisionState BlockNode::getCollisionState(OrbNode* orb) {
	float radius = orb->getRadius();
	glm::vec3 min = actualMin();
	glm::vec3 max = actualMax();
	glm::vec3 center = (max - min) / 2.0f + min;
	glm::vec3 orbCenter = orb->actualCenter();

	//Minkowski sum:
	//uses a new rectangle and checks where orb's centre lies relatively to the new one and its diagonals

	float w = 0.5f * (2 * radius + max.x - min.x);
	float h = 0.5f * (2 * radius + max.y - min.y);
	float dx = orbCenter.x - center.x;
	float dy = orbCenter.y - center.y;

	if (abs(dx) <= w && abs(dy) <= h) {
		float wy = w * dy;
		float hx = h * dx;
		if (wy > hx) {
			if (wy > -hx) {
				return Top;
			}
			return Left;
		}
		if (wy > -hx) {
			return Right;
		}
		return Bottom;
	}
	return None;
}

glm::vec3 BlockNode::getMin() {
	return min;
}

glm::vec3 BlockNode::getMax() {
	return max;
}

glm::vec3 BlockNode::actualMin() {
	return glm::vec3(getWorld() * glm::vec4(this->min, 1.0f));
}

glm::vec3 BlockNode::actualMax() {
	return glm::vec3(getWorld() * glm::vec4(this->max, 1.0f));
}
