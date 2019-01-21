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

void BlockNode::onCollision() {

}

CollisionState BlockNode::getCollisionState(OrbNode* orb) {
	float radius = orb->getRadius();
	glm::vec3 min = glm::vec3(getWorld() * glm::vec4(this->min, 1.0f));
	glm::vec3 max = glm::vec3(getWorld() * glm::vec4(this->max, 1.0f));
	glm::vec3 center = (max - min) / 2.0f + min;
	glm::vec3 orbCenter = glm::vec3(orb->getWorld()[3]);

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

	//float radius = orb->getRadius();
	//glm::vec2 direction = orb->getDirection();
	//glm::vec3 min = glm::vec3(getWorld() * glm::vec4(this->min, 1.0f));
	//glm::vec3 max = glm::vec3(getWorld() * glm::vec4(this->max, 1.0f));
	//glm::vec3 orbCenter = glm::vec3(orb->getWorld()[3]);
	//glm::vec3 center = (max - min) / 2.0f + min;
	//bool left = orbCenter.x - center.x <= 0.0f;
	//bool up = orbCenter.y - center.y >= 0.0f;
	//float xMin = orbCenter.x - radius, xMax = orbCenter.x + radius, yMin = orbCenter.y - radius, yMax = orbCenter.y + radius;
	//if (xMin >= min.x && xMin <= max.x || xMax >= min.x && xMax <= max.x || xMin >= min.x && xMax <= max.x) {
	//	if (up) {
	//		if (yMax <= max.y) {
	//			return Top;
	//		}
	//		if (yMin <= max.y) {
	//			return Top;
	//		}
	//	} else {
	//		if (yMin >= min.y) {
	//			return Bottom;
	//		}
	//		if (yMax >= min.y) {
	//			return Bottom;
	//		}
	//	}
	//}
	//if(yMin >= min.y && yMin <= max.y || yMax >= min.y && yMax <= max.y || yMin >= min.y && yMax <= max.y) {
	//	if (left) {
	//		if (xMax <= min.x) {
	//			return Left;
	//		}
	//		if (xMin <= min.x) {
	//			return Left;
	//		}
	//	} else {
	//		if (xMin >= max.x) {
	//			return Right;
	//		}
	//		if (xMax >= max.x) {
	//			return Right;
	//		}
	//	}
	//}
	return None;
}
