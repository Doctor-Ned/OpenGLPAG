#ifndef DESTROYABLEBLOCKNODE_H
#define DESTROYABLEBLOCKNODE_H

#include "BlockNode.h"

class DestroyableBlockNode : public BlockNode {
public:
	DestroyableBlockNode(MeshBox* mesh, GraphNode* parent = nullptr);
	DestroyableBlockNode(MeshColorBox* mesh, GraphNode* parent = nullptr);
	void onCollision() override;
};

#endif
