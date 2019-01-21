#ifndef ORBNODE_H
#define ORBNODE_H

#include "GraphNode.h"
#include "MeshSphere.h"
#include "BlockNode.h"

class OrbNode : public GraphNode {
public:
	OrbNode(MeshSphere* mesh, float speed, glm::vec2 direction, GraphNode* parent = nullptr);
	void update(double timeDiff) override;
	bool tryCollide(BlockNode *block);
	float getRadius();
protected:
	void bounce(CollisionState state);
	glm::vec2 direction;
	float radius, speed;
};

#endif
