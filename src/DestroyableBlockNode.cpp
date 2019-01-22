#include "DestroyableBlockNode.h"
#include "SceneManager.h"

DestroyableBlockNode::DestroyableBlockNode(MeshBox * mesh, int points, GraphNode * parent) : BlockNode(mesh, parent), points(points) {}
DestroyableBlockNode::DestroyableBlockNode(MeshColorBox* mesh, int points, GraphNode* parent) : BlockNode(mesh, parent), points(points) {}
DestroyableBlockNode::DestroyableBlockNode(MeshRefBox* mesh, int points, GraphNode* parent) : BlockNode(mesh, parent), points(points) {}

void DestroyableBlockNode::onCollision(GameScene *gameScene) {
	parent->removeChild(this);
	gameScene->addPoints(points);
	//TODO: sound? visual effect?
}
