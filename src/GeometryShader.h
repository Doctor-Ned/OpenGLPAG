#ifndef GEOMETRYSHADER_H
#define GEOMETRYSHADER_H

#include "Shader.h"

class GeometryShader : public Shader {
public:
	GeometryShader(char* vertexPath, char* geometryPath, char* fragmentPath);
	void setPointSpaces(std::vector<glm::mat4> pointSpaces);
	void setPointPosition(glm::vec3 pos);
protected:
	void createShaders() override;
	char *geometryPath;
};

#endif
