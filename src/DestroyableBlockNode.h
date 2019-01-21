#ifndef DESTROYABLEBLOCKNODE_H
#define DESTROYABLEBLOCKNODE_H

#include "BlockNode.h"

class DestroyableBlockNode : public BlockNode {
	void onCollision() override;
};

#endif
