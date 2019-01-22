#ifndef DESTROYABLEBLOCKNODE_H
#define DESTROYABLEBLOCKNODE_H

#include "BlockNode.h"
#include "MeshRefBox.h"
class GameScene;

class DestroyableBlockNode : public BlockNode {
public:
	DestroyableBlockNode(MeshBox* mesh, int points, GraphNode* parent = nullptr);
	DestroyableBlockNode(MeshColorBox* mesh, int points, GraphNode* parent = nullptr);
	DestroyableBlockNode(MeshRefBox* mesh, int points, GraphNode* parent = nullptr);
	void onCollision(GameScene *gameScene) override;
protected:
	int points;
};

#endif
