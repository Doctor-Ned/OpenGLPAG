#include "DestroyableBlockNode.h"
DestroyableBlockNode::DestroyableBlockNode(MeshBox * mesh, GraphNode * parent) : BlockNode(mesh, parent) {}
DestroyableBlockNode::DestroyableBlockNode(MeshColorBox* mesh, GraphNode* parent) : BlockNode(mesh, parent) {}

void DestroyableBlockNode::onCollision() {
	parent->removeChild(this);
	//TODO: sound? visual effect?
}
