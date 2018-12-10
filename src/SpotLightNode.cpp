#include "SpotLightNode.h"

SpotLightNode::SpotLightNode(SpotLight * light, Mesh * mesh, GraphNode * parent) : light(light), GraphNode(mesh, parent) {}

void SpotLightNode::update(double timeDiff) {
	light->model = getLocal();
	GraphNode::update(timeDiff);
}

SpotLight * SpotLightNode::getLight() {
	return light;
}
