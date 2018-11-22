#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Headers.h"
#include "Shader.h"

class GeometryShader : public Shader {
public:
	GeometryShader(char *vertexPath, char *fragmentPath, char *geometryPath);
	void setHeight(float height);
protected:
	virtual void createShaders();
	char *geometryPath = NULL;
};

#endif