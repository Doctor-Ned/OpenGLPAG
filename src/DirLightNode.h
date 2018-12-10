#ifndef DIRLIGHTNODE_H
#define DIRLIGHTNODE_H

#include "GraphNode.h"

class DirLightNode : public GraphNode {
public:
	DirLightNode(DirLight *light, Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void update(double timeDiff);
	DirLight *getLight();
protected:
	DirLight *light;
};

#endif