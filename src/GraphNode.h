#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "Headers.h"
#include "Mesh.h"

class GraphNode {
public:
	GraphNode(Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void draw();
	virtual void update(double timeDiff);
	virtual glm::mat4 getLocal();
	virtual glm::mat4 getWorld();
	virtual void setScale(float scale);
	virtual void setLocal(glm::mat4 local);
	virtual void addChild(GraphNode *child);
	virtual void removeChild(GraphNode *child);
protected:
	glm::mat4 local, world;
	std::vector<GraphNode*> children;
	GraphNode *parent;
	Mesh *mesh;
	bool dirty;
	float scale;
};

#endif