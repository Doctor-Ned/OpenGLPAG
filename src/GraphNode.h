#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "Headers.h"
#include "Mesh.h"

class GraphNode {
public:
	GraphNode(Mesh *mesh, GraphNode *parent = NULL);
	void draw();
	glm::mat4 getLocal();
	glm::mat4 getWorld();
	void setLocal(glm::mat4 local);
	void addChild(GraphNode *child);
	void removeChild(GraphNode *child);
private:
	glm::mat4 local, world;
	std::vector<GraphNode*> children;
	GraphNode *parent;
	Mesh *mesh;
	bool dirty;
};

#endif