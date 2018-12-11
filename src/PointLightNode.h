#ifndef POINTLIGHTNODE_H
#define POINTLIGHTNODE_H

#include "GraphNode.h"

class PointLightNode : public GraphNode {
public:
	PointLightNode(PointLight *light, Mesh *mesh = NULL, GraphNode *parent = NULL);
	virtual void update(double timeDiff);
	void drawGui(bool autoUpdate = true);
	PointLight *getLight();
protected:
	PointLight *light;
	bool enabled = true;
	glm::vec4 lastAmbient;
	glm::vec4 lastDiffuse;
	glm::vec4 lastSpecular;
	glm::vec4 pos;
	float constant;
	float linear;
	float quadratic;
	bool lastEnabled;
};

#endif