#include "GraphNode.h"
#include <algorithm>

GraphNode::GraphNode(Mesh * mesh, GraphNode *parent) : mesh(mesh), parent(parent), local(glm::mat4(1.0f)), dirty(true) {}

void GraphNode::draw() {
	if (parent != NULL) {
		dirty |= parent->dirty;
		if (dirty) {
			world = local * parent->getWorld();
			dirty = false;
		}
	} else if (dirty) {
		world = local;
		dirty = false;
	}
	
	if (mesh) {
		
	}
}

glm::mat4 GraphNode::getWorld() {
	return world;
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
	if (std::find(children.begin(), children.end(), child) == children.end() && *children.end() != child) {
		children.push_back(child);
	}
}

void GraphNode::removeChild(GraphNode *child) {
	if (child->parent == this) {
		child->parent = NULL;
	}
	std::remove(children.begin(), children.end(), child);
}