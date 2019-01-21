#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "GraphNode.h"
#include "MeshBox.h"
#include "MeshColorBox.h"
#include "OrbNode.h"

enum CollisionState {
	None,
	Top,
	Bottom,
	Left,
	Right
};

class BlockNode : public GraphNode {
public:
	BlockNode(MeshBox* mesh, GraphNode* parent = nullptr);
	BlockNode(MeshColorBox* mesh, GraphNode* parent = nullptr);
	void update(double timeDiff) override;
	virtual void onCollision();
	CollisionState getCollisionState(OrbNode *orb);
protected:
	glm::vec3 min, max;
};

#endif
