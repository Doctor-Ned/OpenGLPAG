#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include "GraphNode.h"
#include "MeshBox.h"
#include "MeshColorBox.h"
#include "MeshRefBox.h"

class OrbNode;
class GameScene;

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
	BlockNode(MeshRefBox* mesh, GraphNode* parent = nullptr);
	void update(double timeDiff) override;
	virtual void onCollision(GameScene *gameScene);
	CollisionState getCollisionState(OrbNode *orb);
	glm::vec3 getMin();
	glm::vec3 getMax();
	glm::vec3 actualMin();
	glm::vec3 actualMax();
protected:
	glm::vec3 min, max;
};

#endif
