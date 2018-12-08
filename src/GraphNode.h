#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "Headers.h"
#include "Mesh.h"

class GraphNode {
public:
	GraphNode(Mesh *mesh = NULL, GraphNode *parent = NULL);
	void draw();
	virtual void update(double timeDiff);
	glm::mat4 getLocal();
	glm::mat4 getWorld();
	void setScale(float scale);
	void setLocal(glm::mat4 local);
	void addChild(GraphNode *child);
	void removeChild(GraphNode *child);
protected:
	glm::mat4 local, world;
	std::vector<GraphNode*> children;
	GraphNode *parent;
	Mesh *mesh;
	bool dirty;
	float scale;
};

#endif