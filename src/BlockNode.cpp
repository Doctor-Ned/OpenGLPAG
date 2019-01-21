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