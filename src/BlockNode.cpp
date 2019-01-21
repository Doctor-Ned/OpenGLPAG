#include "BlockNode.h"

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

void BlockNode::onCollision() {

}

CollisionState BlockNode::getCollisionState(OrbNode* orb) {
	//top
	float radius = orb->getRadius();
	glm::vec3 orbCenter = glm::vec3(orb->getWorld()[3]);
	glm::vec3 center = (max - min) / 2.0f + min;
	bool left = orbCenter.x - center.x <= 0.0f;
	bool up = orbCenter.y - center.y >= 0.0f;
	float xMin = orbCenter.x - radius, xMax = orbCenter.x + radius, yMin = orbCenter.y - radius, yMax = orbCenter.y + radius;
	if (xMin >= min.x && xMin <= max.x || xMax >= min.x && xMax <= max.x || xMin >= min.x && xMax <= max.x) {
		if (up) {
			if (yMax <= max.y) {
				return Top;
			}
			if (yMin <= max.y) {
				return Top;
			}
		} else {
			if (yMin >= min.y) {
				return Bottom;
			}
			if (yMax >= min.y) {
				return Bottom;
			}
		}
	}
	if(yMin >= min.y && yMin <= max.y || yMax >= min.y && yMax <= max.y || yMin >= min.y && yMax <= max.y) {
		if (left) {
			if (xMax <= min.x) {
				return Left;
			}
			if (xMin <= min.x) {
				return Left;
			}
		} else {
			if (xMin >= max.x) {
				return Right;
			}
			if (xMax >= max.x) {
				return Right;
			}
		}
	}
	return None;
}
