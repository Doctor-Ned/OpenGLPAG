#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Headers.h"
#include "Shader.h"

class GeometryShader : public Shader {
public:
	GeometryShader(char *vertexPath, char *fragmentPath, char *geometryPath);
protected:
	virtual void createShaders();
	char *geometryPath = NULL;
};

#endif