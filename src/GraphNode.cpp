#include "GraphNode.h"
#include <algorithm>

GraphNode::GraphNode(Mesh * mesh, GraphNode *parent) : mesh(mesh), parent(parent), scale(1.0f), local(glm::mat4(1.0f)), dirty(true) {}

void GraphNode::draw() {
	if (parent != NULL) {
		dirty |= parent->dirty;
		if (dirty) {
			world = parent->getWorld() * local;
		}
	} else if (dirty) {
		world = local;
	}

	if (mesh) {
		mesh->draw(world, scale);
	}

	for (int i = 0; i < children.size(); i++) {
		children[i]->draw();
	}
	if (dirty) {
		dirty = false;
	}
}

void GraphNode::update(double timeDiff) {}

glm::mat4 GraphNode::getWorld() {
	return world;
}

void GraphNode::setScale(float scale) {
	this->scale = scale;
}

glm::mat4 GraphNode::getLocal() {
	return local;
}

void GraphNode::setLocal(glm::mat4 local) {
	this->local = local;
	dirty = true;
}

void GraphNode::addChild(GraphNode * child) {
	child->parent = this;
	if (std::find(children.begin(), children.end(), child) == children.end()) {
		children.push_back(child);
	}
}

void GraphNode::removeChild(GraphNode *child) {
	if (child->parent == this) {
		child->parent = NULL;
	}
	std::remove(children.begin(), children.end(), child);
}