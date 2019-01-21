#include "DestroyableBlockNode.h"
void DestroyableBlockNode::onCollision() {
	parent->removeChild(this);
	//TODO: sound? visual effect?
}
