#include "PointLightNode.h"

PointLightNode::PointLightNode(PointLight * light, Mesh * mesh, GraphNode * parent) : light(light), GraphNode(mesh, parent) {}

void PointLightNode::update(double timeDiff) {
	light->model = getLocal();
	GraphNode::update(timeDiff);
}

PointLight * PointLightNode::getLight() {
	return light;
}
