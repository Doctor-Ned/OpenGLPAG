#include "DirLightNode.h"

DirLightNode::DirLightNode(DirLight * light, Mesh * mesh, GraphNode * parent) : light(light), GraphNode(mesh, parent) { }

void DirLightNode::update(double timeDiff) {
	light->model = getLocal();
	GraphNode::update(timeDiff);
}

DirLight * DirLightNode::getLight() {
	return light;
}
