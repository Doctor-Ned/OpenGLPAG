#ifndef DIRLIGHTNODE_H
#define DIRLIGHTNODE_H

#include "GraphNode.h"

class DirLightNode : public GraphNode {
public:
	DirLightNode(DirLight *light, Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void update(double timeDiff);
	void drawGui(bool autoUpdate = true);
	DirLight *getLight();
protected:
	DirLight *light;
	bool enabled = true;
	glm::vec4 lastAmbient;
	glm::vec4 lastDiffuse;
	glm::vec4 lastSpecular;
	float rotationZ;
	float rotationX;
	float appliedZ;
	float appliedX;
	bool lastEnabled;
};

#endif