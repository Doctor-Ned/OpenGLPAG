#include "OrbitingNode.h"

OrbitingNode::OrbitingNode(OrbitNode* orbit, float orbitSpeed, Mesh* mesh)
	: GraphNode(mesh, orbit), angle(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI),
	  orbitSpeed(orbitSpeed)
	  , orbit(orbit) {}

void OrbitingNode::update(double timeDiff) {
	GraphNode::update(timeDiff);
	glm::vec3 moveVec(cos(angle), 0.0f, sin(angle));
	angle += orbitSpeed * timeDiff;
	setLocal(translate(glm::mat4(1.0f), normalize(moveVec) * orbit->getRadius()));
}

void OrbitingNode::setOrbitSpeed(float orbitSpeed) {
	this->orbitSpeed = orbitSpeed;
}

void OrbitingNode::setAngle(float angle) {
	this->angle = angle;
}

float OrbitingNode::getAngle() {
	return angle;
}
